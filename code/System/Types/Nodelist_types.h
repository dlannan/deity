// Nodelist template

#ifndef _NODELIST_TYPES_H_
#define _NODELIST_TYPES_H_

template <class T>
class NodeList
{
private:
    struct Node
    {
        T  data;
        Node* next;
        Node* prev;
    };

    Node* first;
	Node* last;
    Node* current;
public:
    NodeList(void);
    ~NodeList(void);

    void InsertFront(const NodeList<T>& data);
    void InsertBack(const NodeList<T>& data);
	void Delete(Node *node);
	void Delete(int pos);
	void DeleteAll(void);

    T First(void);
    T Next(void);
    T Prev(void);
	T Last(void);
	T Get(int pos);

	int	 Size(void);
	int	 Pos(void);

};






#endif