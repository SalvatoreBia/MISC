#include <stdio.h>
#include <string.h>
#include "misc/queue.h"

typedef struct Customer
{
    int id;
    char name[30];
} Customer;

int main()
{
    // +-----------------------------------+
    // | Simulate a customer service queue |
    // +-----------------------------------+

    misc_queue customer_queue = misc_queue_create(sizeof(Customer));
    if (customer_queue == NULL)
    {
        printf("misc_queue handle allocation failed. Exiting...\n");
        return 1;
    }

    printf("=== Customer Service Queue ===\n\n");

    Customer customers[] = {
        {1, "Alice"},
        {2, "Bob"},
        {3, "Charlie"},
        {4, "Diana"},
        {5, "Eve"}
    };

    printf("Customers arriving at the store...\n");
    for (size_t i = 0; i < 5; i++)
    {
        misc_queue_push(customer_queue, &customers[i]);
        printf("  Customer #%d (%s) joined the queue\n", 
               customers[i].id, customers[i].name);
    }

    printf("\nQueue status:\n");
    printf("  Total customers waiting: %zu\n", misc_queue_size(customer_queue));
    
    Customer *next = (Customer*)misc_queue_front(customer_queue);
    printf("  Next customer: #%d (%s)\n", next->id, next->name);
    
    Customer *last = (Customer*)misc_queue_back(customer_queue);
    printf("  Last in line: #%d (%s)\n\n", last->id, last->name);

    printf("Serving customers...\n");
    int served_count = 0;
    while (!misc_queue_isempty(customer_queue))
    {
        Customer current;
        misc_queue_pop(customer_queue, &current);
        served_count++;
        
        printf("  [%d] Now serving: #%d (%s) - %zu still waiting\n", 
               served_count, current.id, current.name, 
               misc_queue_size(customer_queue));
    }

    printf("\nAll customers served!\n");

    printf("\nNew customer arrives...\n");
    Customer late_customer = {6, "Frank"};
    misc_queue_push(customer_queue, &late_customer);
    printf("  Customer #%d (%s) joined the queue\n", 
           late_customer.id, late_customer.name);
    
    printf("  Queue size: %zu\n", misc_queue_size(customer_queue));

    misc_queue_destroy(customer_queue);
    return 0;
}
