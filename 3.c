
#include <sys/types.h>

typedef struct vector_t vector_t;





/* Include the system headers we need */
#include <stdlib.h>
#include <stdio.h>

#define size_t int
/* Include our header */

/* Define what our struct is */
struct vector_t {
    size_t size;
    int *data;
};

/* Utility function to handle allocation failures. In this
 case we print a message and exit. */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

/* Bad example of how to create a new vector */
vector_t *bad_vector_new() {
    /* Create the vector and a pointer to it */
    vector_t *retval, v;
    retval = &v;
    
    /* Initialize attributes */
    retval->size = 1;
    retval->data = malloc(sizeof(int));
    if (retval->data == NULL) {
        allocation_failed();
    }
    
    retval->data[0] = 0;
    return retval;
}

/* Another suboptimal way of creating a vector */
vector_t also_bad_vector_new() {
    /* Create the vector */
    vector_t v;
    
    /* Initialize attributes */
    v.size = 1;
    v.data = malloc(sizeof(int));
    if (v.data == NULL) {
        allocation_failed();
    }
    v.data[0] = 0;
    return v;
}

/* Create a new vector with a size (length) of 1
 and set its single component to zero... the
 RIGHT WAY */
vector_t *vector_new() {
    /* Declare what this function will return */
    vector_t *retval;
    
    /* First, we need to allocate memory on the heap for the struct */
    retval = (vector_t *)malloc(sizeof(vector_t));
    
    /* Check our return value to make sure we got memory */
    if (!retval) {
        allocation_failed();
    }
    
    /* Now we need to initialize our data.
     Since retval->data should be able to dynamically grow,
     what do you need to do? */
    retval->data = (int *) malloc(sizeof(int));
    
    /* Check the data attribute of our vector to make sure we got memory */
    if (!(retval->data)) {
        free(retval);                //Why is this line necessary?
        allocation_failed();
    }
    
    /* Complete the initialization by setting the single component to zero */
    retval->size = 1;
    
    /* and return... */
    return retval;
}

/* Return the value at the specified location/component "loc" of the vector */
int vector_get(vector_t *v, size_t loc) {
    
    /* If we are passed a NULL pointer for our vector, complain about it and exit. */
    if(v == NULL) {
        fprintf(stderr, "vector_get: passed a NULL vector.\n");
        return 0;
    }
    
    /* If the requested location is higher than we have allocated, return 0.
     * Otherwise, return what is in the passed location.
     */
    if (loc < v->size) {
        return *((v->data)+loc);
    } else {
        return 0;
    }
}

/* Free up the memory allocated for the passed vector.
 Remember, you need to free up ALL the memory that was allocated. */
void vector_delete(vector_t *v) {
    free(v);
    v = NULL;
}



/* Set a value in the vector. If the extra memory allocation fails, call
 allocation_failed(). */
void vector_set(vector_t *v, int loc, int value) {
    /* What do you need to do if the location is greater than the size we have
     * allocated?  Remember that unset locations should contain a value of 0.
     */
    int i;
    if(loc < v->size)
        v->data[loc] = value;
    else{
    int *a = NULL;
    a = (int *)realloc(v->data, (loc + v->size)*sizeof(int));
        v->size += loc;
    v->data = a;
    if(!a)
        allocation_failed();
    v->data[loc] = value;
        for(i=loc+1;i<=v->size-1;i++)
        v->data[i] = 0;
    /* YOUR SOLUTION HERE */
}
}

int main(int argc, char **argv) {
    vector_t *v;
    
    printf("Calling vector_new()\n");
    v = vector_new();
    
    printf("Calling vector_delete()\n");
    vector_delete(v);
    
    printf("vector_new() again\n");
    v = vector_new();
    
    printf("These should all return 0 (vector_get()): ");
    printf("%d ", vector_get(v, 0));
    printf("%d ", vector_get(v, 1));
    printf("%d\n", vector_get(v, 2));
    
    printf("Doing a bunch of vector_set()s\n");
    vector_set(v, 0, 98);
    vector_set(v, 11, 15);
    vector_set(v, 15, -23);
    vector_set(v, 24, 65);
    vector_set(v, 500, 3);
    vector_set(v, 12, -123);
    vector_set(v, 15, 21);
    vector_set(v, 25, 43);
    
    printf("These should be equal:\n");
    printf("98 = %d\n", vector_get(v, 0));
    printf("15 = %d\n", vector_get(v, 11));
    printf("65 = %d\n", vector_get(v, 24));
    printf("-123 = %d\n", vector_get(v, 12));
    printf("21 = %d\n", vector_get(v, 15));
    printf("43 = %d\n", vector_get(v, 25));
    printf("0 = %d\n", vector_get(v, 23));
    printf("0 = %d\n", vector_get(v, 1));
    printf("0 = %d\n", vector_get(v, 501));
    printf("3 = %d\n", vector_get(v, 500));
    
    vector_delete(v);
    
    printf("Test complete.\n");
    
    return 0;
}
