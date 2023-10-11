import random
import matplotlib.pyplot as plt

# Define las funciones de aptitud predefinidas con 1, 2 y 3 variables
def func1(x):
    return x ** 2

def func2(x, y):
    return -x ** 2 + y ** 2 + x*y

def func3(x, y, z):
    return x ** 2 + y ** 2 + z ** 2

# Función para convertir binario a decimal
def binary_to_decimal(binary):
    decimal = 0
    for bit in binary:
        decimal = decimal * 2 + bit
    return decimal

# Crear individuo aleatoriamente
def createIndv(num_variables, num_bits_var):
    return [random.randint(0, 1) for _ in range(num_variables * num_bits_var)]

# Función para evaluar la población
def evaluate_individuos(individuos, func, num_variables, num_bits_var):
    score = []

    for ind in individuos:
        variables = [binary_to_decimal(ind[i:i+num_bits_var]) for i in range(0, len(ind), num_bits_var)]
        score.append(func(*variables))
    return score

# Función para mostrar la tabla de resultados
def print_table(gens, individuos, func, num_variables, num_bits_var):
    score = evaluate_individuos(individuos, func, num_variables, num_bits_var)

    sumatoria = sum(score)

    if sumatoria == 0:
        preseleccion = [0] * len(individuos)
    else:
        preseleccion = [round(resultado / sumatoria, 2) for resultado in score]

    maximo = max(score)
    media = sumatoria / len(individuos)

    # Calcular el valor esperado y actual
    valor_esperado = [round(resultado / int(media), 2) for resultado in score]
    valor_actual = [round(ve,0) for ve in valor_esperado]

    print(f"Generación {gens}:\n")
    print(f"{'Individuo':<15}{'Valores':<15}{'Resultado':<15}{'Preselección':<15}{'Valor Esperado':<15}{'Valor Actual':<15}")

    for i, ind in enumerate(individuos):
        valores = [binary_to_decimal(ind[i:i+num_bits_var]) for i in range(0, len(ind), num_bits_var)]
        resultado = score[i]
        print(f"{''.join(map(str, ind)):<15}{', '.join(map(str, valores)):<15}{resultado:<15}{preseleccion[i]:<15}{valor_esperado[i]:<15}{valor_actual[i]:<15}")

    print(f"\nSumatoria: {sumatoria}")
    print(f"Media: {media}")
    print(f"Máximo: {maximo}\n")
    print("=" * 100 + "\n")

# Función para plotear cada generación
def plotGrafic(value):
    plt.figure()
    plt.plot(range(len(value)), value, marker='o')
    plt.title('Media de cada generación')
    plt.xlabel('Generación')
    plt.ylabel('Media')
    plt.grid(True)
    plt.show()

# Función para realizar la mutación
def mutate(ind1, ind2):
    if len(ind1) != len(ind2):
        raise ValueError("Los individuos deben tener la misma longitud")

    mutated_ind1 = ind1.copy()
    mutated_ind2 = ind2.copy()

    for i in range(len(ind1)):
        if random.random() < 0.05:
            mutated_ind1[i], mutated_ind2[i] = mutated_ind2[i], mutated_ind1[i]

    return mutated_ind1, mutated_ind2

# Función para realizar la mutación en un individuo
def mutate_individual(ind, mutation_rate=0.1):
    mutated_ind = ind.copy()
    for i in range(len(ind)):
        if random.random() < mutation_rate:
            mutated_ind[i] = 1 - mutated_ind[i]
    return mutated_ind

# Algoritmo Genético
def genetic_algorithm(num_gens, n_indv, num_variables, func, num_bits_var, manual_input=False):
    nbits = num_variables * num_bits_var
    value = []

    # Permitir que el usuario ingrese manualmente los individuos iniciales
    if manual_input:
        individuos = []
        for i in range(n_indv):
            print(f"Ingrese el cromosoma para el individuo {i + 1} (de longitud {nbits}):")
            chromosome = input()
            individuos.append([int(bit) for bit in chromosome])
    else:
        # Generar individuos aleatoriamente
        individuos = [createIndv(num_variables, num_bits_var) for _ in range(n_indv)]

    for gens in range(num_gens):
        # Calcular el puntaje (score) dentro del bucle
        score = evaluate_individuos(individuos, func, num_variables, num_bits_var)

        print_table(gens, individuos, func, num_variables, num_bits_var)
        value.append(sum(score) / len(individuos))

        # Selección, cruzamiento y mutación
        new_generation = []

        # Calcular índices de individuos ordenados por puntaje (de mayor a menor)
        sorted_indices = sorted(range(len(score)), key=lambda i: score[i], reverse=True)

        for i in range(n_indv // 2):
            # Selección de dos padres con los puntajes más altos
            parent1 = individuos[sorted_indices[0]]
            parent2 = individuos[sorted_indices[1]]

            # Cruzamiento (crossover)
            crossP = random.randint(1, nbits - 1)
            child1 = parent1[:crossP] + parent2[crossP:]
            child2 = parent2[:crossP] + parent1[crossP:]

            # Mutación
            child1, child2 = mutate(child1, child2)

            # Aplicar mutación a un individuo al azar
            if random.random() < 0.5:
                child1 = mutate_individual(child1)
            else:
                child2 = mutate_individual(child2)

            new_generation.extend([child1, child2])

        individuos = new_generation

    # Dibujar la gráfica de la media de cada generación
    plotGrafic(value)

if __name__ == "__main__":
    num_gens = 100
    n_indv = 5
    num_variables = 2  # Cambia esto para 1, 2 o 3 variables
    num_bits_var = 5  # Cambia esto para ajustar la cantidad de bits por variable
    func = func2  # Cambia esto para usar las funciones de aptitud 1, 2 o 3
    manual_input = False  # Cambia a True si deseas ingresar manualmente los individuos iniciales

    genetic_algorithm(num_gens, n_indv, num_variables, func, num_bits_var, manual_input)
