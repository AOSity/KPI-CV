<p align="center"><b>МОНУ НТУУ КПІ ім. Ігоря Сікорського ФПМ СПіСКС</b></p>
<p align="center">
<b>Звіт з лабораторної роботи 1</b><br/>
"Базова обробка зображень"<br/>
дисципліни "Комп'ютерний Зір"
</p>
<p align="right"><b>Студент:</b> <i>Горбуль Андрій Олександрович КВ-11</i><p>
<p align="right"><b>Рік:</b> <i>2024</i><p>

### Завдання
1. Реалізувати процедуру вирівнювання гістограми півтонового зображення.
2. Реалізувати процедуру умовного масштабування півтонового зображення
(приведення значень середнього та дисперсії одного зображення до значень
дисперсії та середнього іншого зображення).
3. Реалізувати процедуру застосування до півтонового зображення довільного
лінійного локального оператора (заданого ядром фільтра).
4. Реалізувати та порівняти процедури згладжування зображень (видалення
шуму) за допомогою прямокутного фільтра, медіанного фільтра, фільтра
Гаусса та сигма-фільтра (спробувати різні комбінації параметрів, підібрати
оптимальні для тестових зображень). При тестуванні бажано брати
зображення, що містять помітний шум (або додати з цією метою випадковий
шум до незашумлених зображень).
5. Реалізувати підвищення різкості зображень за допомогою нерізкого
маскування (спробувати різні комбінації параметрів, в т.ч. різні операції
згладжування, підібрати оптимальні для тестових зображень). При
тестуванні брати розмиті зображення.
6. Реалізувати та порівняти детектори границь для півтонового зображенні на
основі дискретних похідних, операторів Собеля, Шарра, Лапласса, та
оператора Кенні (спробувати різні значення порогів та підібрати оптимальні
для вхідних зображень). При тестуванні брати зображення, що містять
границі.
7. Реалізувати та порівняти детектори границь LoG та DoG. При тестуванні
брати зображення, що містять границі.

# Збірка та виконання програми

Зібрати та встановити залежності:
```bash
sudo make dependencies
```

Зібрати та запустити проєкт:
```bash
make run [IMAGE=<image_path>]
```
