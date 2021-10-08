typedef struct process_info
{
    int pid;
    int ttl;
} process_info_t;

typedef struct process_node
{
    process_node_t next;
    process_info_t me;
} process_node_t;

typedef struct process_queue
{
    process_node_t first;
    process_node_t last;
} process_queue_t;