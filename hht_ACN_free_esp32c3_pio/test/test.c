int isLarger(int a[], int n1, int b[], int n2) {
    int a_all = 0;
    int b_all = 0;

    for(int i = 0; i < n1; i++) {
        a_all += a[i];
    }

    for(int i = 0; i < n2; i++) {
        b_all += b[i];
    }

    if (a_all > b_all) return 1;
    else return 0;
}
