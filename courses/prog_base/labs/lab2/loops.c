double calc(int n, int m){
    double x = 0, y;
    int i, j;
    for (i = 1; i <= n; i++){
        y = 0;
        for (j = 1; j <= m; j++){
            y += (i*1.0)/(j + 1);
        }
        y *= cos(M_PI/i);
        x +=y;
    }
    return x;
}
