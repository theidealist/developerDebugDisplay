# The minimum set of directives necessary to support deb-helper based packaging

all:
	echo "Invoking scons..."
	scons

clean:
	echo "Invoking scons -c ..."
	scons -c

install:
	echo "Skipping install target, it is done implicitly in target 'all'"

