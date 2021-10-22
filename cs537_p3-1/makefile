all : main.o  text_parser.o graph_builder.o graph_node.o proc_creator.o
	gcc -o 537make main.o  text_parser.o graph_builder.o graph_node.o proc_creator.o -g -lpthread -O0

main.o : main.c text_parser.h graph_builder.h graph_node.h proc_creator.h
	gcc -Wall -Wextra -c main.c -g -lpthread -O0

text_parser.o : text_parser.c text_parser.h graph_builder.h
	gcc -Wall -Wextra -c text_parser.c -g -lpthread -O0

graph_builder.o : graph_builder.c graph_builder.h
	gcc -Wall -Wextra -c graph_builder.c -g -lpthread -O0

graph_node.o : graph_node.c graph_node.h
	gcc -Wall -Wextra -c graph_node.c -g -lpthread -O0

proc_creator.o : proc_creator.c proc_creator.h graph_node.h
	gcc -Wall -Wextra -c proc_creator.c -g -lpthread -O0

clean:
	rm -f 537make *.o
	rm -rf scan-build-out

scan-build: clean
	scan-build -o scan-build-out make

scan-view: scan-build
	firefox -new-window scan-build-out/*/index.html
