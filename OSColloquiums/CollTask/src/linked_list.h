#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <vector>
#include <stdexcept>
#include "core.h"

template<typename T>
struct Node {
T data;
Node* next;
explicit Node(const T& value) : data(value), next(nullptr) {}
~Node() = default;
Node(const Node&) = delete;
Node& operator=(const Node&) = delete;
};

template<typename T>
class LinkedList {
private:
Node<T>* head_;
size_t size_;

static Node<T>* reverseRecursive(Node<T>* current, Node<T>* prev, size_t depth) {
validation::validateRecursionDepth(depth);
if (current == nullptr) {
return prev;
}
Node<T>* next = current->next;
current->next = prev;
return reverseRecursive(next, current, depth + 1);
}

void clearNodes() noexcept {
Node<T>* current = head_;
while (current != nullptr) {
Node<T>* temp = current;
current = current->next;
delete temp;
}
}

public:
LinkedList() : head_(nullptr), size_(0) {}

~LinkedList() noexcept {
try {
clearNodes();
} catch (...) {
}
}

LinkedList(const LinkedList&) = delete;
LinkedList& operator=(const LinkedList&) = delete;

LinkedList(LinkedList&& other) noexcept
: head_(other.head_), size_(other.size_) {
other.head_ = nullptr;
other.size_ = 0;
}

LinkedList& operator=(LinkedList&& other) noexcept {
if (this != &other) {
try {
clearNodes();
} catch (...) {
}
head_ = other.head_;
size_ = other.size_;
other.head_ = nullptr;
other.size_ = 0;
}
return *this;
}

void push_back(const T& value) {
Node<T>* newNode = nullptr;
try {
newNode = new Node<T>(value);
} catch (const std::bad_alloc&) {
throw std::runtime_error("Failed to allocate memory");
}

if (head_ == nullptr) {
head_ = newNode;
size_ = 1;
return;
}

Node<T>* current = head_;
while (current->next != nullptr) {
current = current->next;
}
current->next = newNode;
++size_;
}

void reverse() {
if (head_ == nullptr || head_->next == nullptr) {
return;
}
head_ = reverseRecursive(head_, nullptr, 0);
}

std::vector<T> toVector() const {
std::vector<T> result;
result.reserve(size_);
Node<T>* current = head_;
while (current != nullptr) {
result.push_back(current->data);
current = current->next;
}
return result;
}

bool empty() const noexcept {
return head_ == nullptr;
}

size_t size() const noexcept {
return size_;
}
};

#endif
