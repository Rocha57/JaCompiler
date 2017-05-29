@.strint = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@.strdouble = private unnamed_addr constant [7 x i8] c"%.16E\0A\00"
@.strtrue = private unnamed_addr constant [6 x i8] c"true\0A\00"
@.strfalse = private unnamed_addr constant [7 x i8] c"false\0A\00"

@gcd = global i1 0
define void @gcd2.main(i32 %args.c, i8** %args.v){
	store i1 false, i1* @gcd
	%1 = load i1, i1* @gcd
	%ifcond = icmp eq i1 %1, 1
	br i1 %ifcond , label %then , label %else
then:
	%call0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.strtrue, i32 0, i32 0))
	br label %ifcont
else:
	%call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.strfalse, i32 0, i32 0))
	br label %ifcont
ifcont:
	ret void
}

declare i32 @atoi(i8*)
declare i32 @printf(i8*, ...)
define i32 @main(i32 %argc, i8** %argv){
	%retval = alloca i32
	%argc.addr = alloca i32
	%argv.addr = alloca i8**
	store i32 0, i32* %retval
	store i32 %argc, i32* %argc.addr
	store i8** %argv, i8*** %argv.addr
	%1 = load i32, i32* %argc.addr
	%2 = load i8**, i8*** %argv.addr
	call void @gcd2.main(i32 %1, i8** %2)
	ret i32 0
}
