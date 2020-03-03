.PHONY: run
.PHONY: clean
.PHONY: build

build: Example.class

run: Example.class libIconLookup.so
	java -Djava.library.path=. Example

clean:
	rm *.class
	rm *.so
	rm IconLookup.h

libIconLookup.so: liblinicon_ffi.a linicon_ffi.h IconLookup.c IconLookup.h
	gcc -shared -fPIC IconLookup.c liblinicon_ffi.a -o libIconLookup.so -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -Wall -Wextra

IconLookup.h: IconLookup.java
	javac -h . IconLookup.java

Example.class: Example.java libIconLookup.so
	javac Example.java

