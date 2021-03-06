#ifdef FOO
#define BAR
#ifdef BAR
void bar() {
    return;
}
#else
int bar() {
    return 1;
}
#endif
#endif

int main(void) {
    bar();
    return 0;
}

/*
 * check-name: Complex Conditions
 * check-command: undertaker -j cpppc $file
 * check-output-start
( B0 <-> FOO )
&& ( B1 <-> B0 && BAR. )
&& ( B2 <-> B0 && ( ! (B1) ) )
&& (!B0 -> (BAR <-> BAR.))
&& (B0 -> BAR.)
&& B00
 * check-output-end
 */
