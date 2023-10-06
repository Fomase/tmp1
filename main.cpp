#include <iostream>
#include <algorithm>
#include <list>
#include <string>

using namespace std;

class Editor {
public:
    Editor() {
        iter = text.begin();
    }
    
    // сдвинуть курсор влево
    void Left() {
        if (iter != text.begin()) {
            auto tmpiter = iter;
            --iter;
            swap(*iter, *tmpiter);
        }
    }
    
    // сдвинуть курсор вправо 
    void Right() {
        if (iter != text.end()) {
            auto tmpiter = iter;
            ++iter;
            swap(*tmpiter, *iter);
        }
    }
 
    // вставить символ token
    void Insert(char token) {
        text.insert(iter, token);
    }
    
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1) {
        auto tmpbegin = iter;
        auto tmpend = iter;
        if (tokens >= 1) {
            ++tmpbegin;
        }
        auto dist = min(tokens, static_cast<size_t>(distance(iter, text.end())));
        advance(tmpend, dist);

        if (tmpend != text.end()) {
            ++tmpend;
        }

        buffer.erase(buffer.begin(), buffer.end());
        buffer.insert(buffer.begin(), tmpbegin, tmpend);
        --tmpend;
        text.erase(tmpbegin, tmpend);
        text.erase(tmpend);
    }
    
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1) {
        if (tokens >= 1) {
            ++iter;
        }
        auto dist = min(tokens, static_cast<size_t>(distance(iter, text.end())));
        auto tmpiter = iter;
        advance(tmpiter, dist);
        buffer.erase(buffer.begin(), buffer.end());
        buffer.insert(buffer.begin(), iter, tmpiter);
    }
    
    // вставить содержимое буфера в текущую позицию курсора
    void Paste() {
        text.insert(iter, buffer.begin(), buffer.end());
    }
    
    // получить текущее содержимое текстового редактора
    string GetText() const {
        string output = "";
        for (auto tmpiter = text.begin(); tmpiter != text.end(); ++tmpiter) {
            if (tmpiter != iter) {
                output+= *tmpiter;
            }
        }
        return output;
    }
 
 private:
    list<char> text = {'|'};
    list<char> buffer;
    list<char>::iterator iter = text.begin();
}; 

int main() {
    Editor editor;
    const string text = "hello, world";
    for (char c : text) {
        editor.Insert(c);
    }
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    

    }
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    

    
    editor.Insert(' ');
    
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    cout << editor.GetText() << endl; 
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    cout << editor.GetText() << endl; 
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}
