// 循环计算阶乘
int factorial_loop(int n)
{
    int fn = 1;
    for (int i = 1; i <= n; i++)
        fn = fn * i;
    return fn;
}