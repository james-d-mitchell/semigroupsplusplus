OBJ_DIR = test/bin
TEST_OBJ_DIR = test/bin/test
UTIL_OBJ_DIR = test/bin/util
LOG_DIR = test/logs
LCOV_DIR = test/lcov

TODAY = $(shell date "+%Y-%m-%d-%H-%M-%S")

HEADERS = $(wildcard *.h)  $(wildcard util/*.h)
SOURCES = $(wildcard *.cc) $(wildcard util/*.cc) $(wildcard test/*.cc)

OBJECTS = $(SOURCES:%.cc=$(OBJ_DIR)/%.o)

TEST_PROG = test/test -d yes --force-colour --order lex --abort

CXXFLAGS = -I. -Wall -Wextra -pedantic -Wno-c++11-extensions -std=c++11

COMMON_DOC_FLAGS = --report --merge docs --output html $(SOURCES) $(HEADERS)

ifneq ($(CXX),clang++)
  ifneq ($(CXX), c++) 
    CXXFLAGS += -pthread
   endif
endif

ifneq ($(shell test -e $(OBJ_DIR)/DEBUG && echo exists), exists)
  CLEAN = testclean
else ifneq ($(wildcard $(OBJ_DIR)/*.gcno),)
  CLEAN = testclean
else ifeq ($(shell test -e $(OBJ_DIR)/DEBUG && echo exists), exists)
  CLEAN = testclean
else ifeq ($(shell test -e $(OBJ_DIR)/LCOV && echo exists), exists)
  CLEAN = testclean
else
  CLEAN = $()
endif

ifneq ($(shell test -e $(OBJ_DIR)/DEBUG && echo exists), exists)
  DEBUG_CLEAN = testclean
 else 
  DEBUG_CLEAN = $()
endif

ifneq ($(shell test -e $(OBJ_DIR)/LCOV && echo exists), exists)
  LCOV_CLEAN = testclean
 else 
  LCOV_CLEAN = $()
endif

error:
	@echo "Please choose one of the following: doc, test, testdebug, "
	@echo "testclean, or doclean"; \
	exit 2
doc:
	@echo "Generating static documentation . . ."; \
	cldoc generate $(CXXFLAGS) -- --static $(COMMON_DOC_FLAGS)
	@echo "Fixing some bugs in cldoc . . ."; \
	python docs/cldoc-fix

test: CXXFLAGS += -O2 -g
test: $(CLEAN) testbuild testrun
	rm -f $(OBJ_DIR)/DEBUG
	rm -f $(OBJ_DIR)/LCOV

testquick: CXXFLAGS += -DSKIP_TEST
testquick: test

testdebug: CXXFLAGS += -O0 -g -UNDEBUG -DDEBUG
testdebug: $(DEBUG_CLEAN) testbuild
	touch $(OBJ_DIR)/DEBUG
	rm -f $(OBJ_DIR)/LCOV

testcov: CXXFLAGS += -O0 -g --coverage 
testcov: LDFLAGS = -O0 -g --coverage
testcov: $(LCOV_CLEAN) testdebug testrunquick
	rm -f $(OBJ_DIR)/DEBUG
	touch $(OBJ_DIR)/LCOV
	lcov --capture --directory test/bin --output-file test/lcov/$(TODAY).info
	genhtml test/lcov/$(TODAY).info --output-directory test/lcov/$(TODAY)-html/
	@echo "See: " test/lcov/$(TODAY)-html/index.html

testclean:
	rm -rf $(OBJ_DIR) test/test

docclean:
	rm -rf html

superclean: testclean docclean
	rm -rf $(LOG_DIR) $(LCOV_DIR)

testdirs:
	mkdir -p $(OBJ_DIR) $(TEST_OBJ_DIR) $(UTIL_OBJ_DIR) $(LOG_DIR) $(LCOV_DIR)

$(OBJ_DIR)/%.o: %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

testbuild: testdirs $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o test/test $(LDFLAGS)

testrun:
	@echo "Running the tests ("$(LOG_DIR)/$(TODAY).log") . . ."; \
	test/test -d yes --force-colour --abort | tee -a $(LOG_DIR)/$(TODAY).log
	@( ! grep -q -E "FAILED|failed" $(LOG_DIR)/$(TODAY).log )

.PHONY: error doc test testdebug testcov testclean doclean testdirs testbuild testrun testsuperclean
.NOTPARALLEL: testrun testclean