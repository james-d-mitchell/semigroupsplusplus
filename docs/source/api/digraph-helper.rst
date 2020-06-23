.. Copyright (c) 2020, J. D. Mitchell

   Distributed under the terms of the GPL license version 3.

   The full license is in the file LICENSE, distributed with this software.

Helper functions for ActionDigraph
==================================

Overview
--------

Defined in ``action-digraph-helper.hpp``.

This page contains the documentation for helper function for the class
:cpp:type:`libsemigroups::ActionDigraph`. 

Full API
--------

.. doxygenfunction:: libsemigroups::action_digraph_helper::follow_path
   :project: libsemigroups

.. doxygenfunction:: libsemigroups::action_digraph_helper::is_acyclic(ActionDigraph<T> const&)
   :project: libsemigroups

.. doxygenfunction:: libsemigroups::action_digraph_helper::is_acyclic(ActionDigraph<T> const&, node_type<T> const)
   :project: libsemigroups

.. doxygenfunction:: libsemigroups::action_digraph_helper::is_reachable
   :project: libsemigroups

.. doxygenfunction:: libsemigroups::action_digraph_helper::topological_sort(ActionDigraph<T> const&)
   :project: libsemigroups

.. doxygenfunction:: libsemigroups::action_digraph_helper::topological_sort(ActionDigraph<T> const&, node_type<T> const)
   :project: libsemigroups
    
.. doxygenfunction:: libsemigroups::action_digraph_helper::add_cycle(ActionDigraph<T>&, typename ActionDigraph<T>::const_iterator_nodes, typename ActionDigraph<T>::const_iterator_nodes)
   :project: libsemigroups
    
.. doxygenfunction:: libsemigroups::action_digraph_helper::add_cycle(ActionDigraph<T>&, size_t const)
   :project: libsemigroups
