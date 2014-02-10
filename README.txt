Adam Fowler
ajf2177

Part A)
3.9)During a context switch the kernel must save the state of the currently executing process and load the context of the process which is being switched to. This entails saving the state of the process represented by the PCB in what is known as a state save which includes cpu registers, process state, and memory management information. A state restore is then performed which loads all of the previously mentioned items for the process being switched to.

3.11) The role of the init process in regards to process termination comes into play when a parent process is terminated before it's child process is terminated. The init process will become the parent process of the so called orphan process and call 'wait' so that the child process's entry in the process table and it's process identifier are released as well as the exit value of the child process, otherwise the child process which has not been 'wait'ed on will become what is known as a zombie process.

Part B)
1.)Typeof is actually a compiler extension but is considered an operator despite that. It is replaced by the typename of its parameter at compile time so something like "typeof(int)" would be replaced by "int".
2.) offsetof is a macro which returns the offset of a member given a member and a type.3.) The container_of macro casts a pointer to the type of the member whose containing structure one would like to find in order to assign the address of that member to a variable which is then used to subtract the member variables offset from the address of the pointer, thus giving the address of the containing structure. This macro is used to attain a pointer to the structure which contains a
certain member, such as the list_head used often in the kernel.
4.) The gnu specific extensions used by the container_of macro are the typeof extension used to deduce the type of the member variable which is then used to find the address of the containing structure as well as the gnu compilers extension called statement expressions which allow compound statements surrounded by parentheses and braces to be evaluated similarly to single statement expressions.
5.) The purpose of the first line is to deduce the type of the member variable within the struct for the purpose of declaring the correct pointer to hold that member variables address, which is in turn used to deduce the containing structs address using the offset of the member variable as well as its location in memory it also serves the purpose of type checking(making sure that the type has the member variable as well as making sure ptr is the right type of pointer). I think
it is necessary to have this line for the type checking but if we didn't need to be 100% correct we could probably accomplish the same thing without that line, though we'd miss out on come error checking the compiler could have done for us.


