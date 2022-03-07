int div(int a, int b)
{
#if HAVE_A_ADD_TEN
	a=a+10;
#endif
	return a/b;
}
