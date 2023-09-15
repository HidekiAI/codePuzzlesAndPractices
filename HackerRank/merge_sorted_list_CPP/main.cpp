#include <algorithm>
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class SinglyLinkedListNode {
public:
  int data;
  SinglyLinkedListNode *next;

  SinglyLinkedListNode(int node_data) {
    this->data = node_data;
    this->next = nullptr;
  }
};

class SinglyLinkedList {
public:
  SinglyLinkedListNode *head;
  SinglyLinkedListNode *tail;

  SinglyLinkedList() {
    this->head = nullptr;
    this->tail = nullptr;
  }

  void insert_node(int node_data) {
    SinglyLinkedListNode *node = new SinglyLinkedListNode(node_data);

    if (!this->head) {
      this->head = node;
    } else {
      this->tail->next = node;
    }

    this->tail = node;
  }
};

void print_singly_linked_list(SinglyLinkedListNode *node, string sep,
                              ofstream &fout) {
  while (node) {
    fout << node->data;

    node = node->next;

    if (node) {
      fout << sep;
    }
  }
}

void free_singly_linked_list(SinglyLinkedListNode *node) {
  while (node) {
    SinglyLinkedListNode *temp = node;
    node = node->next;

    free(temp);
  }
}

// Complete the mergeLists function below.

/*
 * For your reference:
 *
 * SinglyLinkedListNode {
 *     int data;
 *     SinglyLinkedListNode* next;
 * };
 *
 */
SinglyLinkedListNode *mergeLists(SinglyLinkedListNode *head1,
                                 SinglyLinkedListNode *head2) {
  // Sample:
  // * Head1 -> 2 -> 10 -> nullptr
  // * Head2 -> 3 -> 7 -> 10 -> 15 -> 30 -> nullptr
  // Want: head -> 2 -> 3 -> 7 -> 10 -> 10 -> 15 -> 30 -> nullptr
  // 1. current_tail = nullptr, head1 = 2, head2 = 3
  // 2. current_tail = 2 -> nullptr, head1 = 10, head2 = 3
  // 3. current_tail = 2 -> 3 -> nullptr, head1 = 10, head2 = 7
  // 4. current_tail = 2 -> 3 -> 7 -> nullptr, head1 = 10, head2 = 10
  // 5. current_tail = 2 -> 3 -> 7 -> 10 -> nullptr, head1 = nullptr, head2 = 10
  // 6. current_tail = 2 -> 3 -> 7 -> 10 -> 10 -> 15 -> 30 -> nullptr, head1 =
  // nullptr, head2 = nullptr note that in step 6, because head1 is nullptr, we
  // just append head2
  auto head = head1 != nullptr && head2 != nullptr
                  ? head1->data > head2->data ? head2 : head1
              : head1 == nullptr ? head2
                                 : head1;
  auto current_tail = head;
  auto current_head1 = head == head1 ? head1->next : head1;
  auto current_head2 = head == head2 ? head2->next : head2;
  current_tail->next = nullptr;

  do {
    if (current_head1 != nullptr && current_head2 != nullptr) {
      // both are not nullptr
      if (current_head1->data > current_head2->data) {
        // head1 is greater than head2, so make links all the way
        // up to the node that is greater than head2
        do {
          // make next the current_head2
          current_tail->next = current_head2;
          current_tail = current_head2;
          // move current_head2 to next
          current_head2 = current_head2->next;
          // make sure to ground the next pointer of tail
          current_tail->next = nullptr;
        } while (current_head2 != nullptr &&
                 current_head2->data < current_head1->data);
      } else {
        // head2 is greater than head1, so make links all the way
        // up to the node that is greater than head1
        do {
          // make next the current_head1
          current_tail->next = current_head1;
          current_tail = current_head1;
          // move current_head1 to next
          current_head1 = current_head1->next;

          // make sure to ground the next pointer of tail
          current_tail->next = nullptr;
        } while (current_head1 != nullptr &&
                 current_head1->data < current_head2->data);
      }
    } else if (current_head1 == nullptr) {
      // head1 is nullptr, just append head2 to current_tail and we're done
      current_tail->next = current_head2;
      current_head2 = nullptr;
    } else if (current_head2 == nullptr) {
      // head2 is nullptr, just append head1 to current_tail and we're done
      current_tail->next = current_head1;
      current_head1 = nullptr;
    }
  } while (current_head1 != nullptr || current_head2 != nullptr);

  return head;
}

void test() {
  // Vector1: 4, 5, 6
  // Vector2: 1, 2, 10
  // Expected: 1, 2, 4, 5, 6, 10
  auto vec1 = vector<int>{4, 5, 6};
  auto vec2 = vector<int>{1, 2, 10};
  auto expected = vector<int>{1, 2, 4, 5, 6, 10};
  SinglyLinkedList *llist1 = new SinglyLinkedList();
  SinglyLinkedList *llist2 = new SinglyLinkedList();
  cout << "vec1:" << endl;
  for_each(vec1.begin(), vec1.end(), [&llist1](const auto &current) {
    llist1->insert_node(current);
    cout << current << ",";
  });
  cout << endl;
  cout << "vec2:" << endl;
  for_each(vec2.begin(), vec2.end(), [&llist2](const auto &current) {
    llist2->insert_node(current);
    cout << current << ",";
  });
  cout << endl;

  // merge the list
  auto result = mergeLists(llist1->head, llist2->head);
  // dump it
  cout << "expected:" << endl;
  for_each(expected.begin(), expected.end(),
           [](const auto &current) { cout << current << " "; });
  cout << endl;
  cout << "result:" << endl;
  while (result) {
    cout << result->data << " ";
    result = result->next;
  }
  cout << endl;
}

int main() {
  test();
  exit(0);

  ofstream fout(getenv("OUTPUT_PATH"));

  int tests;
  cin >> tests;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  for (int tests_itr = 0; tests_itr < tests; tests_itr++) {
    SinglyLinkedList *llist1 = new SinglyLinkedList();

    int llist1_count;
    cin >> llist1_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < llist1_count; i++) {
      int llist1_item;
      cin >> llist1_item;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      llist1->insert_node(llist1_item);
    }

    SinglyLinkedList *llist2 = new SinglyLinkedList();

    int llist2_count;
    cin >> llist2_count;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int i = 0; i < llist2_count; i++) {
      int llist2_item;
      cin >> llist2_item;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      llist2->insert_node(llist2_item);
    }

    SinglyLinkedListNode *llist3 = mergeLists(llist1->head, llist2->head);

    print_singly_linked_list(llist3, " ", fout);
    fout << "\n";

    free_singly_linked_list(llist3);
  }

  fout.close();

  return 0;
}
