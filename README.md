# stars
В этом проекте была создана модель галактики с использованием графического интерфейса Qt.
Здесь единственная сила, влияющая на поведение звезд - сила притяжения.

----
Основные классы: Галактика и Звезда. 
----
Звезды взаимодействуют со звездами, изменяя этим свою скорость. Для ускорения вычислений была использована несколько упрощенная идея Дебаевского радиуса: Галактика делится на квадраты, и, начиная с определенного расстояния (измеряемого в квадратах), звезды взаимодействуют не с отдельными звездами, а с целыми квадратами.
