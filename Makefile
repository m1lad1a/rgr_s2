CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude -fPIC
LDFLAGS = -ldl

SRCDIR = src
INCDIR = include
LIBDIR = lib

TARGET = crypto_tool

# динамические библиотеки
LIBS = \
    $(LIBDIR)/libzeroandone.so \
    $(LIBDIR)/libtableshift.so \
    $(LIBDIR)/libfixedshift.so

# объектные файлы для интерактивного меню
OBJS = $(SRCDIR)/Crypto.o $(SRCDIR)/ZeroAndOne.o $(SRCDIR)/TableShift.o $(SRCDIR)/FixedShift.o

all: $(LIBDIR) $(LIBS) $(TARGET)

$(LIBDIR):
	mkdir -p $(LIBDIR)

# компиляция .о
$(SRCDIR)/ZeroAndOne.o: $(SRCDIR)/ZeroAndOne.cpp $(INCDIR)/ZeroAndOne.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR)/TableShift.o: $(SRCDIR)/TableShift.cpp $(INCDIR)/TableShift.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR)/FixedShift.o: $(SRCDIR)/FixedShift.cpp $(INCDIR)/FixedShift.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRCDIR)/Crypto.o: $(SRCDIR)/Crypto.cpp $(INCDIR)/Crypto.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# создание .so
$(LIBDIR)/libzeroandone.so: $(SRCDIR)/ZeroAndOne.o
	$(CXX) -shared -o $@ $^

$(LIBDIR)/libtableshift.so: $(SRCDIR)/TableShift.o
	$(CXX) -shared -o $@ $^

$(LIBDIR)/libfixedshift.so: $(SRCDIR)/FixedShift.o
	$(CXX) -shared -o $@ $^

# основная программа
$(TARGET): main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET) main.cpp $(OBJS)

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)
	rm -rf $(LIBDIR)
	rm -f *.encrypted *.decrypted

help:
	@echo "make          - собрать проект"
	@echo "make clean    - удалить объектные файлы и .so"
	@echo "make help     - справка"

.PHONY: all clean help
