# Имя итогового исполняемого файла
TARGET = cipher_app

# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Исходные файлы
SRCS = main.cpp src/gronspheld.cpp src/matrix.cpp src/simpleTableCipher.cpp

# Объектные файлы (замена .cpp на .o)
OBJS = $(SRCS:.cpp=.o)

# Цель по умолчанию
all: $(TARGET)

# Линковка
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Компиляция .cpp → .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clear:
	rm -f $(OBJS) $(TARGET)

# Запуск программы
run: all
	./$(TARGET)

# Полная очистка после запуска
run_clear: run
	make clear