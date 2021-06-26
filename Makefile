
INPUT = answer.cpp
OUTPUT = answer

TESTER = ./test.sh
VIS = tools/target/release/vis
TEST_IN = tools/in/0072.txt

TEST_OUT = out.txt


TEST_SHELL = ./test.sh
TEST_PY = ./test.py

CXX = g++-7
CFLAGS = --std=c++17 -Wall -O3
#CFLAGS = --std=c++17 -g -fsanitize=address -Wall
#CFLAGS = --std=c++17 -g -fsanitize=address -Wall -p

.PHONY: test
test: $(OUTPUT)
#	$(TEST_SHELL)
	python $(TEST_PY)

.PHONY: $(TEST_OUT)
$(TEST_OUT): $(OUTPUT)
	./$(OUTPUT) < $(TEST_IN) > $@

.PHONY: svg
svg: $(OUTPUT) $(TEST_OUT)
	$(TESTER) $(TEST_IN) ./$(OUTPUT) tmp
	eog out.svg

$(OUTPUT): $(INPUT)
	$(CXX) $< -o $@ $(CFLAGS)

.PHONY: clean
clean:
	rm $(OUTPUT) $(TEST_OUT)