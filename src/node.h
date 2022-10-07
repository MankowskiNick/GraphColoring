#ifndef NODE_H
#define NODE_H

class Node {
    public:

        Node() {
            Node(-1);
        }

        Node(int initial_id) {
            id = initial_id;
            color = -1;
        }

        int color;
    
        int Id() {
            return id;
        }
        

    private:
        int id;
};

#endif