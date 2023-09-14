#include "graph.h"
#include "graficos.h"

// ------------------------------------------- Parte principal -------------------------------------------

// Establecemos el tamaño del grafo
int graphSize = 50;

// Establecemos el tamaño de la ventana
int windowWidth = 1280;
int windowHeight = 720;

// Creamos el grafo
Graph graph(graphSize, graphSize);

// Creamos un vector de circulos que representan los nodos del grafo
vector<Circle> circles;

// Creamos un vector de lineas que representan las conexiones entre los nodos del grafo
vector<Line> lines;

// Creamos un vector de lineas que representan el camino encontrado por el algoritmo DFS
vector<Line> dfsPath;

// Creamos un vector de lineas que representan el camino encontrado por el algoritmo BFS
vector<Line> bfsPath;

// Creamos un vector de lineas que representan el camino encontrado por el algoritmo A*
vector<Line> aStarPath;

// Creamos un vector de lineas que representan el camino encontrado por el algoritmo Hillclimbling
vector<Line> hillclimbingPath;

// Función que agrega los nodos que existen en el grafo al vector de circulos, los circulos tienen tamaños responsive a la ventana
void addCircles(){
    // Se limpia el vector de circulos
    circles.clear();

    // Se agregan los nodos que existen en el grafo al vector de circulos
    for(int i = 0; i < graphSize; ++i){
        for(int j = 0; j < graphSize; ++j){
            if(graph.nodes[i][j]){
                Circle circle{};
                // Se establece la posición del circulo dependiendo del tamaño de la ventana y el número de nodos
                circle.x = j * windowWidth / graph.cols + windowWidth / graph.cols / 2;
                circle.y = i * windowHeight / graph.rows + windowHeight / graph.rows / 2;
                if(graphSize < 30){
                    circle.radius = 20;
                }else{
                    circle.radius = windowWidth / graph.cols / 2 - 5;
                }

                circle.color[0] = 0;
                circle.color[1] = 0;
                circle.color[2] = 1;
                circles.push_back(circle);
            }
        }
    }
}

// Funcion que halle el indicie de un elemento de un vector
int findIndex(vector<int> v, int element){
    for(int i = 0; i < v.size(); ++i){
        if(v[i] == element){
            return i;
        }
    }
    return -1;
}


// Funcion que agrega las conexiones entre los nodos que existen en el grafo al vector de lineas
void addLines(){
    // Se limpia el vector de lineas
    lines.clear();

    // Se agregan las conexiones entre los nodos que existen en el grafo al vector de lineas
    vector<int>nnn = graph.getNodes();
    for(int i = 0; i < graph.n_nodes; ++i){
        for(int j = 0; j < graph.n_nodes; ++j){
            if(graph.matrix[i][j]){
                Line line;
                int a = findIndex(nnn, i);
                int b = findIndex(nnn, j);
                line.x1 = circles[a].x;
                line.y1 = circles[a].y;
                line.x2 = circles[b].x;
                line.y2 = circles[b].y;
                line.color[0] = 0;
                line.color[1] = 0;
                line.color[2] = 1;
                lines.push_back(line);
            }

        }
    }
}

// Funcion que imprime el menu
void printMenu(){
    // Se muestra el menu infinito en la consola
    cout << "1. Ingresar un nodo inicial y un nodo final como coordenadas para usar el algoritmo DFS" << endl;
    cout << "2. Ingresar un nodo inicial y un nodo final como coordenadas para usar el algoritmo BFS" << endl;
    cout << "3. Ingresar un nodo inicial y un nodo final como coordenadas para usar el algoritmo A*" << endl;
    cout << "4. Ingresar un nodo inicial y un nodo final como coordenadas para usar el algoritmo Hillclimbling" << endl;
    cout << "5. Ingresar un porcentaje de nodos a eliminar del grafo" << endl;
    cout << "6. Restaurar el grafo a su estado inicial" << endl;
    cout << "7. Salir del programa" << endl;
}

// Función que se llama cuando se redimensiona la ventana y cambia el tamaño de la ventana
void resize(int width, int height){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, width, height);
    // Se actualiza el tamaño de la ventana
    windowWidth = width;
    windowHeight = height;
}

// Función que se llama cuando se dibuja la ventana
void display(){

    // Se limpia la pantalla
    glClear(GL_COLOR_BUFFER_BIT);

    // Se dibujan los nodos
    for(int i = 0; i < circles.size(); ++i){
        drawCircle(circles[i]);
    }

    // Se dibujan las conexiones entre los nodos
    for(int i = 0; i < lines.size(); ++i){
        drawLine(lines[i]);
    }

    // Se dibuja el camino encontrado por el algoritmo DFS
    for(int i = 0; i < dfsPath.size(); ++i){
        drawLine(dfsPath[i]);
    }

    // Se dibuja el camino encontrado por el algoritmo BFS
    for(int i = 0; i < bfsPath.size(); ++i){
        drawLine(bfsPath[i]);
    }

    // Se dibuja la escena
    glutSwapBuffers();
}

// Función que se llama cuando se presiona una tecla
void keyboard(unsigned char key, int x, int y){

    // Si se presiona la tecla 1
    if(key == '1'){

        // Se le pide al usuario que ingrese las coordenadas del nodo inicial
        cout << "Ingrese las coordenadas del nodo inicial: ";
        int row1, col1;
        cin >> row1 >> col1;

        // Se le pide al usuario que ingrese las coordenadas del nodo final
        cout << "Ingrese las coordenadas del nodo final: ";
        int row2, col2;
        cin >> row2 >> col2;

        // Se obtiene el camino encontrado por el algoritmo DFS
        vector<int> path = graph.DFS(graph.coord_to_node(row1, col1), graph.coord_to_node(row2, col2));

        // Impresión del camino encontrado por el algoritmo DFS como coordenadas
        for(int i : path){
            cout << i << " ";
        }

        // Se limpia el vector de lineas que representan el camino encontrado por el algoritmo DFS
        dfsPath.clear();
        bfsPath.clear();
        aStarPath.clear();
        hillclimbingPath.clear();


        vector<int>nnn = graph.getNodes();
        // Se agregan las lineas que representan el camino encontrado por el algoritmo DFS con grosor 2
        if(path.size() != 0){
            for(int i = 0; i < path.size() - 1; ++i){
                //cout << "hola";
                Line line;
                int a = findIndex(nnn, path[i]);
                int b = findIndex(nnn, path[i + 1]);
                line.x1 = circles[a].x;
                line.y1 = circles[a].y;
                line.x2 = circles[b].x;
                line.y2 = circles[b].y;
                line.color[0] = 1;
                line.color[1] = 0;
                line.color[2] = 0;
                line.width = 2;
                dfsPath.push_back(line);
            }
        }

        // Se vuelve a dibujar la ventana
        glutPostRedisplay();

        printMenu();
    }

    // Si se presiona la tecla 2
    if(key == '2'){

        // Se le pide al usuario que ingrese las coordenadas del nodo inicial
        cout << "Ingrese las coordenadas del nodo inicial: ";
        int row1, col1;
        cin >> row1 >> col1;

        // Se le pide al usuario que ingrese las coordenadas del nodo final
        cout << "Ingrese las coordenadas del nodo final: ";
        int row2, col2;
        cin >> row2 >> col2;

        // Se obtiene el camino encontrado por el algoritmo BFS
        vector<int> path = graph.BFS(graph.coord_to_node(row1, col1), graph.coord_to_node(row2, col2));
        // Impresión del camino encontrado por el algoritmo BFS como coordenadas de nodos
        for(int i = 0; i < path.size(); ++i){
            cout << path[i] << " ";
        }
        cout << endl;

        // Se limpia el vector de lineas que representan el camino encontrado por el algoritmo BFS
        dfsPath.clear();
        bfsPath.clear();
        aStarPath.clear();
        hillclimbingPath.clear();

        vector<int>nnn = graph.getNodes();
        // Se agregan las lineas que representan el camino encontrado por el algoritmo DFS con grosor 2
        if(path.size() != 0){
            for(int i = 0; i < path.size() - 1; ++i){
                //cout << "hola";
                Line line;
                int a = findIndex(nnn, path[i]);
                int b = findIndex(nnn, path[i + 1]);
                line.x1 = circles[a].x;
                line.y1 = circles[a].y;
                line.x2 = circles[b].x;
                line.y2 = circles[b].y;
                line.color[0] = 1;
                line.color[1] = 0;
                line.color[2] = 0;
                line.width = 2;
                dfsPath.push_back(line);
            }
        }

        // Se vuelve a dibujar la ventana
        glutPostRedisplay();

        printMenu();

    }

    // Si se presiona la tecla 3
    if(key == '3'){

        // Se le pide al usuario que ingrese las coordenadas del nodo inicial
        cout << "Ingrese las coordenadas del nodo inicial: ";
        int row1, col1;
        cin >> row1 >> col1;

        // Se le pide al usuario que ingrese las coordenadas del nodo final
        cout << "Ingrese las coordenadas del nodo final: ";
        int row2, col2;
        cin >> row2 >> col2;

        // Se obtiene el camino encontrado por el algoritmo A*
        vector<int> path = graph.AStar(graph.coord_to_node(row1, col1), graph.coord_to_node(row2, col2));

        // Se limpia el vector de lineas que representan el camino encontrado por el algoritmo A*
        dfsPath.clear();
        bfsPath.clear();
        aStarPath.clear();
        hillclimbingPath.clear();

        vector<int>nnn = graph.getNodes();
        // Se agregan las lineas que representan el camino encontrado por el algoritmo DFS con grosor 2
        if(path.size() != 0){
            for(int i = 0; i < path.size() - 1; ++i){
                //cout << "hola";
                Line line;
                int a = findIndex(nnn, path[i]);
                int b = findIndex(nnn, path[i + 1]);
                line.x1 = circles[a].x;
                line.y1 = circles[a].y;
                line.x2 = circles[b].x;
                line.y2 = circles[b].y;
                line.color[0] = 1;
                line.color[1] = 0;
                line.color[2] = 0;
                line.width = 2;
                dfsPath.push_back(line);
            }
        }
        // Se vuelve a dibujar la ventana
        glutPostRedisplay();

        printMenu();

    }

    // Si se presiona la tecla 4
    if(key == '4'){

        // Se le pide al usuario que ingrese las coordenadas del nodo inicial
        cout << "Ingrese las coordenadas del nodo inicial: ";
        int row1, col1;
        cin >> row1 >> col1;

        // Se le pide al usuario que ingrese las coordenadas del nodo final
        cout << "Ingrese las coordenadas del nodo final: ";
        int row2, col2;
        cin >> row2 >> col2;

        // Se obtiene el camino encontrado por el algoritmo Hillclimbling
        vector<int> path = graph.Hillclimbling(graph.coord_to_node(row1, col1), graph.coord_to_node(row2, col2));

        // Se limpia el vector de lineas que representan el camino encontrado por el algoritmo Hillclimbling
        dfsPath.clear();
        bfsPath.clear();
        aStarPath.clear();
        hillclimbingPath.clear();

        vector<int>nnn = graph.getNodes();
        // Se agregan las lineas que representan el camino encontrado por el algoritmo DFS con grosor 2
        if(path.size() != 0){
            for(int i = 0; i < path.size() - 1; ++i){
                //cout << "hola";
                Line line;
                int a = findIndex(nnn, path[i]);
                int b = findIndex(nnn, path[i + 1]);
                line.x1 = circles[a].x;
                line.y1 = circles[a].y;
                line.x2 = circles[b].x;
                line.y2 = circles[b].y;
                line.color[0] = 1;
                line.color[1] = 0;
                line.color[2] = 0;
                line.width = 2;
                dfsPath.push_back(line);
            }
        }

        // Se vuelve a dibujar la ventana
        glutPostRedisplay();

        printMenu();

    }

    // Si se presiona la tecla 5
    if(key == '5'){

            // Se le pide al usuario que ingrese el porcentaje de nodos a eliminar
            cout << "Ingrese el porcentaje de nodos a eliminar: ";
            float num;
            cin >> num;

            // Se almecenan los pares de nodos que se eliminaron
            graph.removeNodes(num);

            // Se limpian los vectores de circulos y lineas
            circles.clear();
            lines.clear();

            // Se agregan los nodos que existen en el grafo al vector de circulos
            addCircles();
            addLines();

            // Se vuelve a dibujar la ventana
            glutPostRedisplay();

            printMenu();
    }  

    // Si se presiona la tecla 6
    if(key == '6'){

        // Se restaura el grafo a su estado inicial
        graph = Graph(graphSize, graphSize);

        // Se limpian los vectores de circulos, lineas, camino DFS y camino BFS
        circles.clear();
        lines.clear();
        dfsPath.clear();
        bfsPath.clear();

        // Se agregan los nodos que existen en el grafo al vector de circulos
        addCircles();

        // Se agregan las conexiones entre los nodos que existen en el grafo al vector de lineas
        addLines();

        // Se vuelve a dibujar la ventana
        glutPostRedisplay();

        printMenu();

    }

    // Si se presiona la tecla 7
    if(key == '7'){

        // Se cierra la ventana
        glutDestroyWindow(glutGetWindow());

    }

}



// Función principal
int main(int argc, char **argv){

    // Se inicializa la ventana
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Grafo");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);

    // Se agregan los nodos que existen en el grafo al vector de circulos con tamaños responsive a la ventana
    addCircles();

    // Se agregan las conexiones entre los nodos que existen en el grafo al vector de lineas
    addLines();

    // Se imprime el menu
    printMenu();

    // Se inicia el loop de OpenGL
    glutMainLoop();

    return 0;
}
