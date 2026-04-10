#include <iostream>
#include <string>

using namespace std;



//Логика для стека
struct Stack {
    char value;
    Stack* next;
};

Stack* stack_init () {
    Stack* stack = nullptr;
    return stack;
}

Stack* stack_push (Stack* beg, char value) {
    Stack* newElem = new Stack;
    newElem->next = beg;
    newElem->value = value;
    return newElem;
}

char stack_get (Stack*& beg) {
    if (!beg) {
        cout << "Стек пуст" << '\n';
        return '\0';
    }
    char ch = beg->value;
    Stack* buf = beg->next;
    delete beg;
    beg = buf;
    return ch;
}

char stack_top(Stack* stack) {
    if (stack != nullptr) return stack->value;
    return '\0';
}

void stck_print (Stack* point) {
    while (point) {
        cout << point->value << ' ';
        point = point->next;
    }
    cout << '\n';
}

//Логика для очереди
struct Queue {
    string value;
    Queue* prev;
    Queue* next;
};

void queue_push(Queue*& start, Queue*& end, string value) {
    Queue* newElem = new Queue;
    newElem->value = value;
    newElem->next = nullptr;
    newElem->prev = end;

    if (end != nullptr) {
        end->next = newElem;
    } else {
        start = newElem; 
    }
    end = newElem;
}

string queue_get (Queue*& start) {
    if (!start) {
        cout << "Очередь пуста";
        return "";
    }
    Queue* buf = start->next;
    string value = start->value;
    delete start;
    start = buf;
    start->prev = nullptr;
    return value;
}

void queue_print (Queue* point) {
    while (point) {
        cout << point->value << ' ';
        point = point->next;
    }
    cout << '\n';
}

//Остальное 
int weight (char ch) {
    switch (ch) {
        case '+': case '-':
            return 1;
        case '*': case '/':
            return 2;
        case '(': case ')':
            return 0;
        default:
            return -1;
    }
}

void change_variables (string &s) {
    int i = 0; 
    string t = "0123456789+-*/()";
    while (i<s.length()) {
        if (t.find(s[i]) == string::npos) {
            cout << "Инициализируйте переменную " << s[i] << '\n';
            string buf;
            getline(cin, buf);
            s.replace(i, 1 , buf);
            i += buf.length()-1;
        }
        else {
            i++;
        }
    }
}

bool is_correct_simple (string s) {
    int signCount = 0, numCount = 0, unMinusC = 0, openCount = 0, closeCount = 0;
    string num = "";
    for (int i=0; i<=s.length(); i++) {
        if (s[i] == '-' || s[i] == '+' || s[i] == '*' || s[i] == '/')
            signCount++;

        if (s[i] >= '0' && s[i] <= '9') {
            num += s[i];
        }
        else if (num != ""){
            numCount++;
            num = "";
        }

        if (s[i] == '(')
            openCount++;
        if (s[i] == ')')
            closeCount++;
        
        if (i!=0 && (s[i] == '+' || s[i] == '/' || s[i] == '*') && (s[i-1] == '-' || s[i-1] == '+' || s[i-1] == '/' || s[i-1] == '*')) {
            cout << " Ошибка: Невозможное расположение знаков\n";
            return false;
        }

        if (i!=s.length() && s[i] == '-' && (i == 0 || s[i-1] == '-' || s[i-1] == '+' || s[i-1] == '*' || s[i-1] == '/') && s[i+1] >= '0' && s[i+1] <= '9') {
            unMinusC++;
        }
    }

    if (signCount - unMinusC != numCount-1) {
        cout << "Ошибка: Невозможное расположение знаков\n";
        return false;
    }
    if (openCount != closeCount) {
        cout << "Ошибка: Невозможное расположение скобок\n";
        return false;
    }
    return true;
}

bool is_correct_polish (string s, Queue* start) {
    int signCount = 0, numCount = 0, unMinusC = 0;
    for (int i=0; i<=s.length(); i++) {
        if (s[i] == '-' || s[i] == '+' || s[i] == '*' || s[i] == '/')
            signCount++;
        if (i!=s.length() && s[i] == '-' && (i == 0 || s[i-1] == '-' || s[i-1] == '+' || s[i-1] == '*' || s[i-1] == '/') && s[i+1] >= '0' && s[i+1] <= '9') {
            unMinusC++;
        }
    }
    Queue* point = start;
    while (point != nullptr) {
        numCount++;
        point = point->next;
    }
    if (signCount - unMinusC != numCount-1) {
        cout << "Ошибка: Невозможное расположение знаков\n";
        return false;
    }
    return true;
}

//Reverse Polish Notation
string RPN (Stack*& stack, Queue*& queueStart, Queue*& queueEnd, string s, bool LOG_FLAG = false) {
    if (LOG_FLAG) cout << "Преобразование в обратную польскуюб нотацию:\n";
    string num = "";
    string rpn = "";
    for (int i=0; i<=s.length(); i++) {
        if (s[i] >= '0' && s[i] <= '9' || (s[i] == '-' && (i == 0 || s[i-1] == '-' || s[i-1] == '+' || s[i-1] == '*' || s[i-1] == '/') && s[i+1] >= '0' && s[i+1] <= '9')) {
            num += s[i];
            rpn +=s[i];
        }
        else if (num != ""){
            queue_push(queueStart, queueEnd, num);
            if (LOG_FLAG) cout << "Добавлние числа " << num << " в очередь\n";
            num = "";
        }

        if (weight(s[i])>0 && i != 0 && s[i-1] != '-' && s[i-1] != '+' && s[i-1] != '*' && s[i-1] != '/') {
            while (stack != nullptr && weight(stack_top(stack)) >= weight(s[i])) {
                char value = stack_get(stack);
                if (LOG_FLAG) cout << "Взятие знака " << value << " из стека\n";
                rpn += value;
                if (LOG_FLAG) cout << "Добавление знака " << value << " в ответ \n";
            }
            stack = stack_push(stack, s[i]);
            if (LOG_FLAG) cout << "Добавление знака " << s[i] << " в стек\n";
        }
        else if (s[i] == '(') {
            stack = stack_push(stack, s[i]);
            if (LOG_FLAG) cout << "Добавление знака " << s[i] << " в стек\n";
        }
        else if (s[i] == ')') {
            while (stack != nullptr && stack_top(stack) != '(') {
                char value = stack_get(stack);
                if (LOG_FLAG) cout << "Взятие знака " << value << " из стека\n";
                rpn += value;
                if (LOG_FLAG) cout << "Добавление знака " << value << " в ответ \n";
            }
            char velur = stack_get(stack);
            cout << "Удаление ( из стека\n";
        }
    
    }
    while (stack != nullptr) {
        char buf = stack_get(stack) ;
        rpn += buf;
        if (LOG_FLAG) cout << "Добавления занка " << buf << " в ответ\n";
    }
    if (LOG_FLAG) cout << "Обратная польская нотация: " << rpn << '\n';
    return rpn;
}

//Direct Polish Notation
string DPN (Stack*& stack, Queue*& queueStart, Queue*& queueEnd, string s, bool LOG_FLAG = false) {
    if (LOG_FLAG) cout << "Преобразование в прямую польскуюб нотацию:\n";
    string num = "";
    string dpn = "";
    for (int i=s.length(); i>=0; i--) {
        if (s[i] >= '0' && s[i] <= '9' || (s[i] == '-' && (i == 0 || s[i-1] == '-' || s[i-1] == '+' || s[i-1] == '*' || s[i-1] == '/') && s[i+1] >= '0' && s[i+1] <= '9')) {
            num = s[i] + num;
            dpn = s[i] + dpn;
        }
        else if (num != ""){
            queue_push(queueStart, queueEnd, num);
            if (LOG_FLAG) cout << "Добавлние числа " << num << " в очередь\n";
            num = "";
        }

        if (weight(s[i])>0 && i != 0 && s[i-1] != '-' && s[i-1] != '+' && s[i-1] != '*' && s[i-1] != '/') {
            while (stack != nullptr && weight(stack_top(stack)) >= weight(s[i])) {
                char value = stack_get(stack);
                if (LOG_FLAG) cout << "Взятие знака " << value << " из стека\n";
                dpn = value + dpn;
                if (LOG_FLAG) cout << "Добавление знака " << value << " в ответ \n";
            }
            stack = stack_push(stack, s[i]);
            if (LOG_FLAG) cout << "Добавление знака " << s[i] << " в стек\n";
        }
        else if (s[i] == '(') {
            stack = stack_push(stack, s[i]);
            if (LOG_FLAG) cout << "Добавление знака " << s[i] << " в стек\n";
        }
        else if (s[i] == ')') {
            while (stack != nullptr && stack_top(stack) != '(') {
                char value = stack_get(stack);
                if (LOG_FLAG) cout << "Взятие знака " << value << " из стека\n";
                dpn = value + dpn;
                if (LOG_FLAG) cout << "Добавление знака " << value << " в ответ \n";
            }
            char velur = stack_get(stack);
            cout << "Удаление ( из стека\n";
        }
    
    }
    while (stack != nullptr) {
        char buf = stack_get(stack) ;
        dpn = buf + dpn;
        if (LOG_FLAG) cout << "Добавления занка " << buf << " в ответ\n";
    }
    if (LOG_FLAG) cout << "Прямая польская нотация: " << dpn << '\n';
    return dpn;
}

int main() {
    const bool LOG_FLAG = false;


    Stack* stack = stack_init();
    if (LOG_FLAG) cout << "Инициализация стека" <<'\n';
    Queue* queueStart = nullptr;
    Queue* queueEnd = nullptr;
    if (LOG_FLAG) cout << "Инициализация очереди" <<'\n';

    cout << "Введите ваше выражение:\n";
    string s = "";
    getline(cin, s);
    cout << s << '\n';
    
    change_variables (s);

    string rpn = RPN(stack, queueStart, queueEnd, s);
    string dpn = DPN(stack, queueStart, queueEnd, s);

    cout << rpn << ' ' << dpn << '\n';

    if(LOG_FLAG) cout << "Проверка на правильность выражения\n";
    if (is_correct_simple(s)) {
        if(LOG_FLAG) cout << "Успешно\n";
    }
    else {
        if(LOG_FLAG) cout << "В выражении ошибка, программа будет прервана\n";
        return 0;
    }

    cout << RPN(stack, queueStart, queueEnd, s, LOG_FLAG) << '\n';

    if (is_correct_polish(s, queueStart)) {
         cout << "Успешно\n";
    }
    else {
         cout << "В выражении ошибка, программа будет прервана\n";
        return 0;
    }
    return 0;
}