
/*
 * C library -- _setjmp, _longjmp
 *
 *	_longjmp(a,v)
 * will generate a "return(v?v:1)" from the last call to
 *	_setjmp(a)
 * by restoring registers from the stack.
 * The previous signal state is NOT restored.
 */

.text
.globl setjmp
setjmp:
	sw ra, 0(a0)
	sw sp, 4(a0)
	sw gp, 8(a0)
	sw tp, 12(a0)
	sw s0, 16(a0)
	sw s1, 20(a0)
	sw s2, 24(a0)
	sw s3, 28(a0)
	sw s4, 32(a0)
	sw s5, 36(a0)
	sw s6, 40(a0)
	sw s7, 44(a0)
	sw s8, 48(a0)
	sw s9, 52(a0)
	sw s10, 56(a0)
	sw s11, 60(a0)
	li a0, 0 // return 0;
	ret

.globl longjmp
longjmp:
	lw ra, 0(a0)
	lw sp, 4(a0)
	lw gp, 8(a0)
	lw tp, 12(a0)
	lw s0, 16(a0)
	lw s1, 20(a0)
	lw s2, 24(a0)
	lw s3, 28(a0)
	lw s4, 32(a0)
	lw s5, 36(a0)
	lw s6, 40(a0)
	lw s7, 44(a0)
	lw s8, 48(a0)
	lw s9, 52(a0)
	lw s10, 56(a0)
	lw s11, 60(a0)

	li	a0, 1			/* default return value */
	beqz	a1, 1f			/* test return value */
	move	a0, a1			/* use it if not 0 */
1:	ret