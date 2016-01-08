#ifndef CIRCULAR_QUEUE_H_
#define CIRCULAR_QUEUE_H_

#define QUEUE_SIZE 100

typedef struct
{
	unsigned char queue[QUEUE_SIZE];
	unsigned int read_index;
	unsigned int write_index;
	int read_and_write_on_same_lap;//bool
} circular_queue;

void circular_queue_construct(volatile circular_queue *q);
int circular_queue_is_empty(volatile circular_queue *q);//bool
unsigned char circular_queue_read_next_char(volatile circular_queue *q);
void circular_queue_write_char(volatile circular_queue *q, unsigned char c);



#endif /* CIRCULAR_QUEUE_H_ */
