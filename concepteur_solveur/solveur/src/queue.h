
typedef struct Queue{
    int *tab;
    int first;
    int last;
    int queueMaxSize;
    int queueNbElement;
} Queue;

void NewQueue(Queue**queue, int initialQueueSize);
// teste si la file est vide

bool isQueueEmpty(Queue*queue);
// teste si la file est pleine

bool isQueueFull(Queue*queue);
// ajoute un élément dans la file

int queue(Queue*queue, int value);
// sort un élément de la file

int deQueue(Queue*queue, int*value);