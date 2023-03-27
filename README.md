# Мультиплатформенное выделение динамической памяти

# Структура разметки кучи
При инициализации кучи - в память помещается структура инициализации  
Каждый участок выделяемой памяти имеет вид - служебный заголовок + пользовательские данные  

![Image](https://github.com/TakitaNirasa/heap_impl/blob/main/doc/heap_scheme.drawio.png)

# Описание содержимого структур

![Image](https://github.com/TakitaNirasa/heap_impl/blob/main/doc/heap_t.drawio.png)

Для сборки проекта ввести: 
```make all```  
Для сборки тестов и генерации файла покрытия:  
```make coverage```
