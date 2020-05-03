struct node {
    node(int k){
        prior = rand();
        size = 1;
        value = k;
        left = NULL;
        right = NULL;
        mn = value;
    }
    node *left;
    node *right;
    int value;
    int prior;
    int size;
    int mn;
};

void update(node *n) {
    if(n == NULL) return;
    else if (n->left != NULL && n->right != NULL) {
        n->size = n->left->size + n->right->size + 1;
        n->mn = min(n->value, n->left->mn);
        n->mn = min(n->mn, n->right->mn);
    }
    else if (n->left == NULL && n->right != NULL){
        n->size = n->right->size + 1;
        n->mn = min(n->value, n->right->mn);
    }
    else if (n->left != NULL && n->right == NULL){
        n->size = n->left->size + 1;
        n->mn = min(n->value, n->left->mn);
    }
    else {
        n->size = 1;
        n->mn = n->value;
    }
}

pair<node *, node *> split(node *tr, int k, int add = 0) {
    int current_index;
    if (tr == NULL) {
        return {NULL, NULL};
    }
    if (tr->left != NULL) current_index = add + tr->left->size + 1;
    else current_index = 1 + add;

    if (k > current_index) {
        pair<node *, node *> tmp = split(tr->right, k, current_index);
        tr->right = tmp.x;
        update(tr);
        return {tr, tmp.y};
    } else {
        pair<node *, node *> tmp = split(tr->left, k, add);
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
    } else if (tr1->prior > tr2->prior) {
        tr1->right = merge(tr1->right, tr2);
        update(tr1);
        return tr1;
    } else {
        tr2->left = merge(tr1, tr2->left);
        update(tr2);
        return tr2;
    }
}

node* insert(node *tr, node* k, int pos) {
    auto tmp = split(tr, pos);
    tmp.x = merge(tmp.x, k);
    tr = merge(tmp.x, tmp.second);
    update(tr);
    return tr;
}

pair<node*, int> min(node* tr, int l, int r) {
    int ans = 0;
    auto tmp = split(tr, l);
    auto tmp_tmp = split(tmp.second, r - l + 2);
    if (tmp_tmp.first != NULL) ans = tmp_tmp.first->mn;

    tmp.second = merge(tmp_tmp.first, tmp_tmp.second);
    tr = merge(tmp.first, tmp.second);

    return {tr, ans};
}

void out(node* tr){
    for(int i = 0; i < n; i++){
        auto tmp = split(tr, 2);
        std::cout << tmp.x->value << ' ' << std::flush;
        tr = tmp.y;
    }
    std::cout << '\n';
}

