This program is a quick and dirty code randomizer.

Usage:  randomizer infile outfile

It looks for two preprocessor directives:

#define RANDOMCODE
and
#define ENDRANDOMCODE

Anything between these two directives is randomized on a per line basis.

Example:
	cout <<"test1"<<endl;
	cout <<"test2"<<endl;
	cout <<"test3"<<endl;
becomes
	cout <<"test1"<<endl;
	cout <<"test3"<<endl;
	cout <<"test2"<<endl;

WARNING.  This is why that per line thing is important:
	cout <<"test1"<<endl;  cout <<"test2"<<endl;
	cout <<"test3"<<endl;
becomes:
	cout <<"test3"<<endl;
	cout <<"test1"<<endl;  cout <<"test2"<<endl;

also, it does not handle things like if statements.
	if(true == a)
	cout << "a is true" << endl;
can become:
	cout << "a is true" << endl;
	if(true == a)