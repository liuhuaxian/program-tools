int add(int a, int b)
{
#if HAVE_ADD_MODIFY
	a=a+10;
#endif
	return a+b;
}
