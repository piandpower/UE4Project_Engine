/*
    Copyright 2005-2012 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

#define HARNESS_NO_PARSE_COMMAND_LINE 1
#include "harness.h"

#include "../tbb/intrusive_list.h"

using tbb::internal::intrusive_list_node;

// Machine word filled with repeated pattern of FC bits
const uintptr_t NoliMeTangere = ~uintptr_t(0)/0xFF*0xFC;

struct VerificationBase : Harness::NoAfterlife {
    uintptr_t m_Canary;
    VerificationBase () : m_Canary(NoliMeTangere) {}
};

struct DataItemWithInheritedNodeBase : intrusive_list_node {
    int m_Data;
public:
    DataItemWithInheritedNodeBase ( int value ) : m_Data(value) {}

    int Data() const { return m_Data; }
};

class DataItemWithInheritedNode : public VerificationBase, public DataItemWithInheritedNodeBase {
    friend class tbb::internal::intrusive_list<DataItemWithInheritedNode>;
public:
    DataItemWithInheritedNode ( int value ) : DataItemWithInheritedNodeBase(value) {}
};

struct DataItemWithMemberNodeBase {
    int m_Data;
public:
    // Cannot be used by member_intrusive_list to form lists of objects derived from DataItemBase
    intrusive_list_node m_BaseNode;

    DataItemWithMemberNodeBase ( int value ) : m_Data(value) {}

    int Data() const { return m_Data; }
};

class DataItemWithMemberNodes : public VerificationBase, public DataItemWithMemberNodeBase {
public:
    intrusive_list_node m_Node;

    DataItemWithMemberNodes ( int value ) : DataItemWithMemberNodeBase(value) {}
};

typedef tbb::internal::intrusive_list<DataItemWithInheritedNode> IntrusiveList1;
typedef tbb::internal::memptr_intrusive_list<DataItemWithMemberNodes, 
        DataItemWithMemberNodeBase, &DataItemWithMemberNodeBase::m_BaseNode> IntrusiveList2;
typedef tbb::internal::memptr_intrusive_list<DataItemWithMemberNodes, 
        DataItemWithMemberNodes, &DataItemWithMemberNodes::m_Node> IntrusiveList3;

const int NumElements = 256 * 1024;

//! Iterates through the list forward and backward checking the validity of values stored by the list nodes
template<class List, class Iterator>
void CheckListNodes ( List& il, int valueStep ) {
    int i;
    Iterator it = il.begin();
    for ( i = valueStep - 1; it != il.end(); ++it, i += valueStep ) {
        ASSERT( it->Data() == i, "Unexpected node value while iterating forward" );
        ASSERT( (*it).m_Canary == NoliMeTangere, "Memory corruption" );
    }
    ASSERT( i == NumElements + valueStep - 1, "Wrong number of list elements while iterating forward" );
    it = il.end();
    for ( i = NumElements - 1, it--; it != il.end(); --it, i -= valueStep ) {
        ASSERT( (*it).Data() == i, "Unexpected node value while iterating backward" );
        ASSERT( it->m_Canary == NoliMeTangere, "Memory corruption" );
    }
    ASSERT( i == -1, "Wrong number of list elements while iterating backward" );
}

template<class List, class Item>
void TestListOperations () {
    typedef typename List::iterator iterator;
    List il;
    for ( int i = NumElements - 1; i >= 0; --i )
        il.push_front( *new Item(i) );
    CheckListNodes<const List, typename List::const_iterator>( il, 1 );
    iterator it = il.begin();
    for ( ; it != il.end(); ++it ) {
        Item &item = *it;
        it = il.erase( it );
        delete &item;
    }
    CheckListNodes<List, iterator>( il, 2 );
    for ( it = il.begin(); it != il.end(); ++it ) {
        Item &item = *it;
        il.remove( *it++ );
        delete &item;
    }
    CheckListNodes<List, iterator>( il, 4 );
}

#include "harness_bad_expr.h"

template<class List, class Item>
void TestListAssertions () {
#if TRY_BAD_EXPR_ENABLED
    tbb::set_assertion_handler( AssertionFailureHandler );
    List il1, il2;
    Item n1(1), n2(2), n3(3);
    il1.push_front(n1);
    TRY_BAD_EXPR( il2.push_front(n1), "only one intrusive list" );
    TRY_BAD_EXPR( il1.push_front(n1), "only one intrusive list" );
    il2.push_front(n2);
    TRY_BAD_EXPR( il1.remove(n3), "not in the list" );
    tbb::set_assertion_handler( NULL );
#endif /* TRY_BAD_EXPR_ENABLED */
}

int TestMain () {
    TestListOperations<IntrusiveList1, DataItemWithInheritedNode>();
    TestListOperations<IntrusiveList2, DataItemWithMemberNodes>();
    TestListOperations<IntrusiveList3, DataItemWithMemberNodes>();
    TestListAssertions<IntrusiveList1, DataItemWithInheritedNode>();
    TestListAssertions<IntrusiveList2, DataItemWithMemberNodes>();
    TestListAssertions<IntrusiveList3, DataItemWithMemberNodes>();
    return Harness::Done;
}
