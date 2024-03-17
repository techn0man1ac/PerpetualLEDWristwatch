[![UA_version_README](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/Flags/UA%402x.png)](https://github.com/techn0man1ac/PerpetualLEDWristwatch/blob/main/README_UA.md)
[![GB_version_README](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/Flags/GB%402x.png)](https://github.com/techn0man1ac/PerpetualLEDWristwatch)

# "Вічний" наручний годиник на 12-ти світлодіодах

![Device_face](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/Face.jpg)

Простий та "вічний" наручний годиник з 12-ма світлодіодами, має LIR2430 Li-Ion батарею та 12 мікро сонячних панелей, оснований на простому мікроконтролері від Microchip ATtimy13. Цей проєкт повністю open source(залізо та прошивка).

![PCB_back](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/Back.jpg)

# Відео з демонстрацією:

https://youtu.be/T3b3lKKN2u0

# Як це працює?

Кожні 0.5 секунди процесор прокидається(час ітерації циклу 100 наносекунд), інкрементує змінну мілісекунд, та швиденько засипає, весь цей процес забирає споживання від 3 до 5 мкА. Наручний годиник має кнопку "NOW" та 12 світлодіодів, якщо натиснути кнопку - показується час на світлодіодах згідно з наступним алгоритмом - спочатку показуються години(1, 2, 3, 4, 5, 6 і так далі...) і потім показуються хвилини з кроком в 5 хв(5, 10, 15, 20, 25, 30 і так далі...), час показу пів секунди для годин та стільки ж для хвилин -> знову сон.

# 3D рендер плати:

Лицьова сторона плати:

![PCB_Up_3D_Render](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/PCB_face_3D_render.png)

Зворотня сторона плати:

![PCB_Down_side_3D_Render](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/PCB_back_3D_render.png)

Плата має 35 мм в діаметрі:

![PCB_size](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/PCB_size.png)

Товщина плати має бути 0.8 мм, інакше просто не поміститься в корпус.

# Gerber файли та 3D модель:

Файли для виготовлення:

https://github.com/techn0man1ac/PerpetualLEDWristwatch/tree/main/PCB

# Сторінка плати:

Свіжа версія плати завжди тут:

https://oshwlab.com/raznie.podelki/attiny85binarywatches_copy

# Схемотехніка:

Пристрій має просту схемотехніку, для економії виходів мікроконтролера я використав Мультиплексування методом Чарлі(сумарно 35 компонентів):

https://en.wikipedia.org/wiki/Charlieplexing

![Schematic_Wristwatch](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/PCB/Schematic/Schematic_12LEDWachATtiny13_2024-03-03.png)

В цьому проєкті, для заряджання, я використав 12 фотодіодів BPW34S, у сонячну погоду один елемент генерує 0.5-0.6 В і ~2 мА, з'єднав їх послідовно та зробив просту зарядну схему використовуючи 2 компоненти - стабілітрон ZD1 знижує напругу з 6 до 4.7 В та наявний захист від "витоку" струму в сонячні панелі з допомогою діоду D1 LL4148(саме цей має низький зворотній струм):

![1N4148_parameters](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/1N4148_parameters.png)

Діод добавляє падіння напруги в 0.7(4.7-0.7=4.0 В), відповідно, акумулятор не перезарядиться більше 4 В.

# Як запрограмувати мікроконтролер ATtiny

![AVR ISP programmer pinout](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/ProgramingPins.png)

Ось розпіновка для підключення до AVR ISP програматора - відпаяйте батарею і запаяйте на ніжку 3V 5 Вольт з програматора, MI->MISO програматора(Arduino пін 11), MO->MOSI(пін 12), CK->SCK(пін 13) RS->Reset(пін 10) та GD-> земля програматора(GND).

Як запрограмувати ATtiny13 використовуючи Arduino:

https://www-hackster-io.translate.goog/taunoerik/programming-attiny13-with-arduino-uno-07beba?_x_tr_sl=auto&_x_tr_tl=uk&_x_tr_hl=uk&_x_tr_pto=wapp

![FLASH](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/FLASH.png)

Параметри для ATtiny13:

- Плата: ATtiny 13

- BOD: BOD Disabled

- Частота: 1.2 MHz internal osc.

- Compiler LTO: Enable

- Порт: (Ваш Arduino порт)

- Програматор: Arduino as ISP

![Arduino IDE](https://raw.githubusercontent.com/techn0man1ac/PerpetualLEDWristwatch/main/Imgs/ArduinoIDE.png)

Вихідний код(для Arduino IDE - "12LEDWachATtiny13.ino") ось тут:

https://github.com/techn0man1ac/PerpetualLEDWristwatch/tree/main/Code/12LEDWachATtiny13

Цей проєкт є повністю відкритим, як плата так і прошивка для мікроконтролера, тому його модернізувати може кожен. Але треба віддати належне попередньому проєкту, без якого цього б не існувало "Бінарний наручний годиник":

https://github.com/techn0man1ac/ATtiny13BinaryWatches

# Місія та візія проєкту

Місія:

"Розробити простий і екологічно чистий годинник, який поєднує в собі інноваційні технології з мінімальною кількістю компонентів, гарантуючи, що енергоефективність доступна для всіх.".

Візія:

"Ми прагнемо створити світ, де енергозберігаючі технології будуть доступні кожному, де наш годинник буде символом піклування про навколишнє середовище та інноваційних рішень у сфері енергоефективності."
