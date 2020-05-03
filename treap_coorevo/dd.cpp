struct node {
    node(int k){
        key = k;
        value = rand();
        sum = key;
        left = NULL;
        right = NULL;
    }
    node *left;
    node *right;
    int key;
    int value;
    int sum;
};

void update(node *n) {
    if(n == NULL) return;
    else if (n->left != NULL && n->right != NULL) n->sum = n->key + n->right->sum + n->left->sum;
    else if (n->left == NULL && n->right != NULL) n->sum = n->key + n->right->sum;
    else if (n->left != NULL && n->right == NULL) n->sum = n->key + n->left->sum;
    else n->sum = n->key;
}

pair<node *, node *> split(node *tr, int k) {
    if (tr == NULL) {
        return {NULL, NULL};
    } else if (k > tr->key) {
        pair<node *, node *> tmp = split(tr->right, k);
        tr->right = tmp.x;
        update(tr);
        return {tr, tmp.y};
    } else {
        pair<node *, node *> tmp = split(tr->left, k);
        tr->left = tmp.y;
        update(tr);
        return {tmp.x, tr};
    }
}

node *merge(node *tr1, node *tr2) {
    if (tr2 == NULL) {
        return tr1;
    }
    if (tr1 == NULL) {
        return tr2;
    } else if (tr1->value > tr2->value) {
        tr1->right = merge(tr1->right, tr2);
        update(tr1);
        return tr1;
    } else {
        tr2->left = merge(tr1, tr2->left);
        update(tr2);
        return tr2;
    }
}

node* insert(node *tr, node* k) {
    auto tmp = split(tr, k->key);
    auto tr1 = merge(tmp.x, k);
    tr = merge(tr1, tmp.second);
    update(tr);
    return tr;
}

pair<node*, int> sum(node* tr, int l, int r) {
    int ans = 0;
    auto tmp = split(tr, l);
    auto tmp_tmp = split(tmp.second, r+1);
    if (tmp_tmp.first != NULL) ans = tmp_tmp.first->sum;
 
    tmp.second = merge(tmp_tmp.first, tmp_tmp.second);
    tr = merge(tmp.first, tmp.second);
 
    return {tr, ans};
}

bool find(node* tr, int k){
    if (tr == NULL){
        return false;
    }
    if (tr->key == k){
        return true;
    }
    else if (tr->key < k) {
        return find(tr->right, k);
    }
    else {
        return find(tr->left, k);
    }
}