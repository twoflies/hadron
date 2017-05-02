CC=g++
CFLAGS=-g -Wall -fPIC -L. -Wl,-rpath,. `pkg-config gtkmm-3.0 --cflags`

ODIR=obj

LIBS=`pkg-config gtkmm-3.0 --libs`

_OBJ = widget.o container.o layout.o easing.o animated.o depthblur.o ring.o rbutton.o font.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cc %.h
	$(CC) -c -o $@ $< $(CFLAGS)

libhadron: $(OBJ)
	$(CC) -shared -o $@.so $^ $(CFLAGS) $(LIBS)

lib: libhadron
	cp libhadron.so lib/
	cp *.h lib/include/hadron

test: $(ODIR)/test.o
	$(CC) -o test $^ $(CFLAGS) $(LIBS) -lhadron -lfontconfig

all: libhadron test

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core libhadron.so lib/libhadron.so lib/include/hadron/*.h
