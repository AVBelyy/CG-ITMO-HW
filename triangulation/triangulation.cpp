#include <cstdint>
#include <cstdio>
#include <vector>

using namespace std;
int n;

// point

struct point {
    int x;
    int y;
    int line;

    point() : x(0), y(0), line(-1) {}
    point(int x, int y, int line) : x(x), y(y), line(line) {}
};

struct triangle {
    int a;
    int b;
    int c;
    
    triangle() : a(0), b(0), c(0) {}
    triangle(int a, int b, int c) : a(a), b(b), c(c) {}
};

// doubly-linked cyclic list

struct list {
    struct node;
    struct iterator;
    struct const_iterator;

    list();
    list(list const& other);
    ~list();

    list& operator=(list other);
    bool empty() const;
    void push_back(point value);
    point& back();
    point const& back() const;
    void pop_back();
    void push_front(point value);
    point& front();
    point const& front() const;
    void pop_front();
    void insert(iterator pos, point value);
    void erase(iterator pos);
    void splice(iterator pos, list& other, iterator first, iterator last);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    private:
    node *root;
};

struct list::node {
    list::node* prev;
    list::node* next;
    point value;
};

struct list::iterator {
    friend class list;

    iterator(node* ptr);

    point& operator*() const;
    iterator& operator++();
    iterator  operator++(int);
    iterator& operator--();
    iterator  operator--(int);

    friend bool operator==(list::iterator const& a, list::iterator const& b);
    friend bool operator!=(list::iterator const& a, list::iterator const& b);

    private:
    node* ptr;
};

bool operator==(list::iterator const& a, list::iterator const& b);
bool operator!=(list::iterator const& a, list::iterator const& b);

struct list::const_iterator {
    friend class list;

    const_iterator(node* ptr);

    point const& operator*() const;
    const_iterator& operator++();
    const_iterator  operator++(int);
    const_iterator& operator--();
    const_iterator  operator--(int);

    friend bool operator==(list::const_iterator const& a, list::const_iterator const& b);
    friend bool operator!=(list::const_iterator const& a, list::const_iterator const& b);

    private:
    node* ptr;
};

bool operator==(list::const_iterator const& a, list::const_iterator const& b);
bool operator!=(list::const_iterator const& a, list::const_iterator const& b);

list::list() {
    root = new list::node;
    root->value = point();
    root->prev = root->next = root;
}

list::list(list const& other) {
    root = new list::node;
    root->value = point();
    root->prev = root->next = root;
    for (list::const_iterator it = other.begin(); it != other.end(); ++it) {
        push_back(*it);
    }
}

list::~list() {
    list::node *tmp, *cur = root->next;
    while (cur != root) {
        tmp = cur;
        cur = cur->next;
        delete tmp;
    }
    delete root;
}

list& list::operator=(list other) {
    std::swap(other.root, root);
    return *this;
}

bool list::empty() const {
    return root->next == root;
}

void list::push_back(point value) {
    insert(iterator(root), value);
}

point& list::back() {
    return root->prev->value;
}

point const& list::back() const {
    return root->prev->value;
}

void list::pop_back() {
    erase(iterator(root->prev));
}

void list::push_front(point value) {
    insert(iterator(root->next), value);
}

point& list::front() {
    return root->next->value;
}

point const& list::front() const {
    return root->next->value;
}

void list::pop_front() {
    erase(iterator(root->next));
}

void list::insert(iterator pos, point value) {
    list::node* pos_node = pos.ptr;
    list::node* new_node = new list::node;
    new_node->value = value;
    pos_node->prev->next = new_node;
    new_node->prev = pos_node->prev;
    new_node->next = pos_node;
    pos_node->prev = new_node;
}

void list::erase(iterator pos) {
    list::node* pos_node = pos.ptr;
    pos_node->prev->next = pos_node->next;
    pos_node->next->prev = pos_node->prev;
    delete pos_node;
}

void list::splice(iterator pos, list& other, iterator first, iterator last) {
    list::node *pos_node = pos.ptr;
    list::node *first_node = first.ptr, *last_node = last.ptr;
    list::node *tmp = first_node->prev;
    pos_node->prev->next = first_node;
    first_node->prev = pos_node->prev;
    last_node->prev->next = pos_node;
    pos_node->prev = last_node->prev;
    tmp->next = last_node;
    last_node->prev = tmp;
}

typename list::iterator list::begin() {
    return iterator(root->next);
}

typename list::iterator list::end() {
    return iterator(root);
}

typename list::const_iterator list::begin() const {
    return const_iterator(root->next);
}

typename list::const_iterator list::end() const {
    return const_iterator(root);
}

// list::iterator

list::iterator::iterator(node* ptr) : ptr(ptr) {}

point& list::iterator::operator*() const {
    return ptr->value;
}

list::iterator& list::iterator::operator++() {
    ptr = ptr->next;
    if (ptr->value.line == -1) ptr = ptr->next;
    return *this;
}

list::iterator list::iterator::operator++(int) {
    list::node* res = ptr;
    ptr = ptr->next;
    if (ptr->value.line == -1) ptr = ptr->next;
    return iterator(res);
}

list::iterator& list::iterator::operator--() {
    ptr = ptr->prev;
    if (ptr->value.line == -1) ptr = ptr->prev;
    return *this;
}

list::iterator list::iterator::operator--(int) {
    list::node* res = ptr;
    ptr = ptr->prev;
    if (ptr->value.line == -1) ptr = ptr->prev;
    return iterator(res);
}

bool operator==(list::iterator const& a, list::iterator const& b) {
    return a.ptr == b.ptr;
}

bool operator!=(list::iterator const& a, list::iterator const& b) {
    return a.ptr != b.ptr;
}


// list::const_iterator

list::const_iterator::const_iterator(node* ptr) : ptr(ptr) {}

point const& list::const_iterator::operator*() const {
    return ptr->value;
}

typename list::const_iterator& list::const_iterator::operator++() {
    ptr = ptr->next;
    if (ptr->value.line == -1) ptr = ptr->next;
    return *this;
}

typename list::const_iterator list::const_iterator::operator++(int) {
    list::node* res = ptr;
    ptr = ptr->next;
    if (ptr->value.line == -1) ptr = ptr->next;
    return const_iterator(res);
}

typename list::const_iterator& list::const_iterator::operator--() {
    ptr = ptr->prev;
    if (ptr->value.line == -1) ptr = ptr->prev;
    return *this;
}

typename list::const_iterator list::const_iterator::operator--(int) {
    list::node* res = ptr;
    ptr = ptr->prev;
    if (ptr->value.line == -1) ptr = ptr->prev;
    return const_iterator(res);
}

bool operator==(list::const_iterator const& a, list::const_iterator const& b) {
    return a.ptr == b.ptr;
}

bool operator!=(list::const_iterator const& a, list::const_iterator const& b) {
    return a.ptr != b.ptr;
}

// end of doubly-linked cyclic list

list::iterator prev(list::iterator it) {
    return --it;
}

list::iterator next(list::iterator it) {
    return ++it;
}

bool operator==(const point & a, const point & b) {
    return a.line == b.line;
}

bool operator!=(const point & a, const point & b) {
    return a.line != b.line;
}

int turn(point & a, point & b, point & c) {
    int64_t result = ((int64_t)b.x - a.x) * ((int64_t)c.y - a.y) - ((int64_t)b.y - a.y) * ((int64_t)c.x - (int64_t)a.x);
    return result < 0 ? -1 : (result > 0 ? 1 : 0);
}

bool inTriangle(point & a, point & b, point & c, point & v) {
    int t1 = turn(a, b, v), t2 = turn(b, c, v), t3 = turn(c, a, v);
    if (t1 == 0 || t2 == 0 || t3 == 0) {
        return t1 + t2 + t3 != 0;
    }
    return t1 == t2 && t2 == t3;
}

bool isConcave(list::iterator p) {
    return turn(*prev(p), *p, *next(p)) <= 0;
}

vector<point> findConcave(list::iterator it) {
    vector<point> res;
    list::iterator p0 = it;
    do {
        if (isConcave(it)) {
            res.push_back(*it);
        }
        ++it;
    } while (p0 != it);
    return res;
}

bool isAnEar(list::iterator p) {
    vector<point> concaves = findConcave(p);

    if (concaves.size() == 0) {
        return true;
    }
    if (isConcave(p)) {
        return false;
    }
    for (point r : concaves) {
        if (inTriangle(*prev(p), *p, *next(p), r) && *prev(p) != r && *p != r && *next(p) != r) {
            return false;
        }
    }
    return true;
}

vector<triangle> triangulate(list & points) {
    vector<triangle> res;
    auto p = points.begin(), p0 = p;
    int cnt = n;
    ++p; ++p;
    
    while (p != p0) {
        if (isAnEar(prev(p)) && cnt >= 3) {
            res.push_back({(*prev(prev(p))).line, (*prev(p)).line, (*p).line});
            points.erase(prev(p));
            cnt--;
            if(p0 == prev(p)) {
                ++p;
            }
        } else {
            ++p;
        }
    }

    return res;
}

int main() {
    scanf("%d", &n);
    
    list p;
    for (int i = 0; i < n; i++) {
        int x, y;
        scanf("%d%d", &x, &y);
        p.push_back({x, y, i + 1});
    }

    for (auto t : triangulate(p)) {
        printf("%d %d %d\n", t.a, t.b, t.c);
    }
    
    return 0;
}
