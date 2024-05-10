#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node* left;
    struct node* right;
    int ht; // tinggi node
};
struct node* root = NULL;

struct node* rotate_left(struct node* root);
struct node* rotate_right(struct node* root);
int balance_factor(struct node* root);
int height(struct node* root);
struct node* create_node(int data);
struct node* insert(struct node* root, int data);
struct node* delete_node(struct node* root, int data);
struct node* minValueNode(struct node* node);
struct node* search(struct node* root, int data);

struct node* rotate_left(struct node* root) {
    struct node* right_child = root->right;
    root->right = right_child->left;
    right_child->left = root;

    // update the heights of the nodes
    root->ht = height(root);
    right_child->ht = height(right_child);

    // return the new node after rotation
    return right_child;
}

struct node* rotate_right(struct node* root) {
    struct node* left_child = root->left;
    root->left = left_child->right;
    left_child->right = root;

    // update the heights of the nodes
    root->ht = height(root);
    left_child->ht = height(left_child);

    // return the new node after rotation
    return left_child;
}

int balance_factor(struct node* root) {
    int lh, rh;

    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL) {
        lh = 0;
    } else {
        lh = 1 + root->left->ht;
    }
    if (root->right == NULL) {
        rh = 0;
    } else {
        rh = 1 + root->right->ht;
    }
    return lh - rh;
}

int height(struct node* root) {
    int lh, rh;

    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL) {
        lh = 0;
    } else {
        lh = 1 + root->left->ht;
    }
    if (root->right == NULL) {
        rh = 0;
    } else {
        rh = 1 + root->right->ht;
    }
    if (lh > rh)
        return lh;
    return rh;
}

struct node* create_node(int data) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = data;
    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
}

struct node* insert(struct node* root, int data) {
    if (root == NULL) {
        struct node* new_node = create_node(data);
        root = new_node;
    } else if (data > root->data) {
        root->right = insert(root->right, data);

        // jika tidak seimbang, lakukan rotasi
        if (balance_factor(root) == -2) {
            if (data > root->right->data) {  // RR
                root = rotate_left(root);
            } else {  // RL
                root->right = rotate_right(root->right);
                root = rotate_left(root);
            }
        }
    } else {
        root->left = insert(root->left, data);

        if (balance_factor(root) == 2) {
            // LL
            if (data < root->left->data) {
                root = rotate_right(root);
            }
            // LR
            else {
                root->left = rotate_left(root->left);
                root = rotate_right(root);
            }
        }
    }
    // update tinggi node
    root->ht = height(root);
    return root;
}

struct node* delete_node(struct node* root, int data) {
    if (root == NULL) {
        return root;
    }
    if (data < root->data) {
        root->left = delete_node(root->left, data);
    } else if (data > root->data) {
        root->right = delete_node(root->right, data);
    } else {
        // node dengan satu atau tanpa anak
        if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        }

        // node dengan dua anak: ambil nilai terkecil dari subtree kanan
        struct node* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = delete_node(root->right, temp->data);
    }

    // update tinggi node
    root->ht = height(root);

    // lakukan rotasi jika diperlukan
    if (balance_factor(root) == 2) {
        // LL
        if (balance_factor(root->left) >= 0) {
            root = rotate_right(root);
        } else {  // LR
            root->left = rotate_left(root->left);
            root = rotate_right(root);
        }
    } else if (balance_factor(root) == -2) {
        // RR
        if (balance_factor(root->right) <= 0) {
            root = rotate_left(root);
        } else {  // RL
            root->right = rotate_right(root->right);
            root = rotate_left(root);
        }
    }
    return root;
}

struct node* minValueNode(struct node* node) {
    struct node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct node* search(struct node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (root->data < data) {
        return search(root->right, data);
    }
    return search(root->left, data);
}

// inorder traversal of the tree
void inorder(struct node* root) {
    if (root == NULL) {
        return;
    }

    inorder(root->left);
    printf("%d ", root->data);
    inorder(root->right);
}

int main() {
    root = insert(root, 10);
    root = insert(root, 15);
    root = insert(root, 20);
    root = insert(root, 9);
    root = insert(root, 5);
    root = insert(root, 16);
    root = insert(root, 17);
    root = insert(root, 8);
    root = insert(root, 6);

    // Contoh penggunaan fungsi
    printf("Inorder traversal sebelum penghapusan: ");
    inorder(root);
    printf("\n");

    root = delete_node(root, 15);
    printf("Inorder traversal setelah penghapusan: ");
    inorder(root);
    printf("\n");

    struct node* result = search(root, 20);
    if (result != NULL) {
        printf("Data 20 ditemukan dalam AVL tree.\n");
    } else {
        printf("Data 20 tidak ditemukan dalam AVL tree.\n");
    }

    return 0;
}
