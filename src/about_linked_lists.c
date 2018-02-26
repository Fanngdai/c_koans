#include "c_koans.h"
#include <stdlib.h>

/*
    Linked list is a collection of elements, typically represented
    through a node referencing another node by means of a pointer.
    This data structure allows for efficient insertion/removal of
    elements from any node in the list.

    Unlike an array where elements are sequential, elements of a
    linked list may not be next to each other in memory.
 */

/*
    Simplest form of a linked list structure.
    Each list node holds a integer value and a pointer to the next node
    in the list.
 */
struct linked_list {
    // 4 bytes
    int data;
    // 8 bytes
    struct linked_list *next;
};

Test(about_linked_lists, linked_list_basics)
{
    /*
        Here we declare two linked_list structs.
        `head` has a pointer to `list_node`.
    */
    struct linked_list head = { 10 };
    struct linked_list list_node = { 12 };

    head.next = &list_node;

    // What ever that address is...
    cr_assert_eq(head.next, &list_node, "What is the `next` value of head?");
    cr_assert_eq(head.next->data, 12, "What `data` does it hold?");

    /*
        Remember that when using the sizeof operator on a array type, it will
        return the sum of all the memory the array uses.

        How much memory does each `struct linked_list` node take up? What are
        the pros/cons of this memory usage compared to arrays?
    */
    cr_assert_eq(sizeof(head), 16, "How much memory does `head` occupy?");
}

Test(about_linked_lists, traversing_linked_list)
{
    /*
        Here we declare 5 linked_list structs with nodes_i pointing to nodes_i-1
    */
    struct linked_list nodes[5] = { { 1 }, { 2, &nodes[0] }, { 3, &nodes[1] },
        { 4, &nodes[2] }, { 5, &nodes[3] } };

    /*
        You typically want to keep reference of where the head of your list is.
        Use a temporary variable to iterate through the nodes.
    */
    struct linked_list *temp_head = &nodes[4];

    /* Traversing a linked list using a while loop. */
    while (temp_head) {
        /* Do some operations ... */
        temp_head = temp_head->next;
    }

    /* Traversing a linked list using a for loop. */
    for (struct linked_list *i = &nodes[4]; i; i = i->next) {
        /* Do some operations ... */
    }

    cr_assert_eq(nodes[3].next, &nodes[2], "What is nodes[3] pointing to?");
    cr_assert_eq(nodes[3].next->data, 3, "What is the data contained?");
}

/*
    An example of how one might append a node to a linked list.
    Some linked list implementations set the new node as the head
    of the list. This makes insertion efficient, but at the cost
    of element ordering.
*/
struct linked_list *insert(struct linked_list *head, int val)
{
    struct linked_list *new_node, *temp;

    for (temp = head; temp && temp->next; temp = temp->next)
        ;

    new_node = malloc(sizeof(*new_node));
    // 2
    new_node->data = val;
    new_node->next = NULL;

    if (temp)
        temp->next = new_node;

    return new_node;
}

Test(about_linked_lists, insert_into_linked_list)
{
    struct linked_list head = { 1 };

    /*
        It's nice to create functions to insert/delete from linked list.
        Make sure to keep track of allocated memory.
    */
    struct linked_list *new_node = insert(&head, 2);

    cr_assert_eq(new_node->next, NULL, "What is the new node pointing to?");
    cr_assert_eq(head.next, new_node, "Where is head.next pointing to?");
    cr_assert_eq(
        head.next->data, 2, "What data is stored in the node after `head`?");
}

/*
    Contrived example of a deletion from a linked list.
    Return the node to delete.
*/
struct linked_list *delete (struct linked_list *head, int val)
{
    struct linked_list *prev, *i;

    prev = NULL;
    // nodes[4]
    i = head;
    while (i && i->data != val) {
        // val = 3 nodes[2]
        prev = i;
        // nodes[3]
        i = i->next;
    }

    if (prev && i)
        prev->next = i->next;

    return i;
}

Test(about_linked_lists, delete_from_linked_list)
{
    struct linked_list nodes[5] = { { 1, NULL }, { 2, &nodes[0] },
        { 3, &nodes[1] }, { 4, &nodes[2] }, { 5, &nodes[3] } };

    /* Do something with removed node .. */
    struct linked_list *deleted_node = delete (&nodes[4], 3);

    // nodes[2] -> nodes[1]
    cr_assert_eq(
        deleted_node->next, &nodes[1], "What was the deleted node pointing to?");
    //
    cr_assert_eq(nodes[3].next, &nodes[1], "What is the next node after nodes[3]?");
    cr_assert_eq(
        nodes[3].next->data, 2, "What data is contained in that node?");
}

Test(about_linked_lists, helpful_pointers)
{
    /* It is useful to define pointers to the start and end of a linked list. */
    struct list_node {
        int data;
        struct list_node *next;
    };

    struct _linked_list {
        struct list_node *head;
        struct list_node *tail;
    };

    cr_assert_eq(
        sizeof(struct _linked_list), 16, "How much memory does it occupy?");

    /*
        nodes[0] = 5    <- tail
        nodes[1] = 4
        nodes[2] = 3
        nodes[3] = 2
        nodes[4] = 1    <- head
    */
    struct list_node nodes[5] = { { 5, NULL }, { 4, &nodes[0] },
        { 3, &nodes[1] }, { 2, &nodes[2] }, { 1, &nodes[3] } };

    struct _linked_list list = { &nodes[4], &nodes[0] };

    cr_assert_eq(list.head, &nodes[4], "Where is head pointing to?");
    cr_assert_eq(
        list.head->data, 1, "What is the data contained in the head?");

    cr_assert_eq(list.tail, &nodes[0], "Where is tail pointing to?");
    cr_assert_eq(
        list.tail->data, 5, "What is the data contained in the tail?");
}

Test(about_linked_lists, doubly_linked_list)
{
    /*
        Doubly linked list is similar to a regular linked list except each node
        has a pointer to the previous node in the list.
        We can reduce deletion time because we can easily modify the prev/next
        pointers of the node to be deleted.
    */
    struct dbl_linked_list {
        int data;
        struct dbl_linked_list *next;
        struct dbl_linked_list *prev;
    };

    cr_assert_eq(sizeof(struct dbl_linked_list), 24,
        "How much memory does it occupy?");

    struct dbl_linked_list n1 = { 1 }, n2 = { 2 }, n3 = { 3 };
    n1.next = &n2;
    n2.next = &n3;
    n2.prev = &n1;
    n3.prev = &n2;


    // n1 -> n2 -> n3
    // n3 <- n2 <- n1
    cr_assert_eq(n2.prev, &n1, "What is the `prev` node of n2?");
    cr_assert_eq(n2.next, &n3, "What is the `next` node of n2?");
}
