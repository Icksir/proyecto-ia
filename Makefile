# Nombre del programa de salida
TARGET = GVRP

# Directorios de origen, encabezados y compilación
SRCDIR = src
INCDIR = include
APPDIR = app
BUILDDIR = build

# Compilador y opciones
CXX = g++
CXXFLAGS = -Wall
CPPFLAGS = -I$(INCDIR)

# Archivos fuente y objetos
SRCS = $(wildcard $(SRCDIR)/*.cpp)
SRCS += $(wildcard $(APPDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(APPDIR)/main.cpp,$(SRCS)))
OBJS += $(patsubst $(APPDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter $(APPDIR)/main.cpp,$(SRCS)))

# Reglas de compilación
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(APPDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Crea la carpeta de compilación si no existe
$(shell mkdir -p $(BUILDDIR))

# Limpieza
clean:
	rm -rf $(BUILDDIR) $(TARGET)

example:
	./GVRP instances/AB101.dat 1000
