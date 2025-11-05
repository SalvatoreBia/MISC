MISC - C Data Structures Library
==================================

A modern, generic data structures library for C featuring vectors, stacks, linked lists, queues, and hash tables.

Overview
--------

MISC provides efficient, reusable data structures with clean APIs suitable for any C project.
All structures use opaque handles and void pointers for type-generic storage.

.. toctree::
   :maxdepth: 2
   :caption: API Reference:

   vector
   stack
   list
   queue
   htable

Features
--------

* **Vector**: Dynamic array with automatic growth
* **Stack**: LIFO stack implementation
* **Linked List**: Singly linked list with tail optimization
* **Queue**: FIFO queue implementation
* **Hash Table**: Hash table with chaining and automatic rehashing

Quick Start
-----------

.. code-block:: c

   #include "misc/vector.h"

   misc_vector vec = misc_vector_create(sizeof(int));
   int value = 42;
   misc_vector_pushback(vec, &value);
   misc_vector_destroy(vec);

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`

