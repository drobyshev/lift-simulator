# lift-simulator
Симулятор лифта.

## Работа с программой
Программа запускается командой:
```
./Lift <count> <height> <velocity> <timeout-doors>
```
, где
* `count` - количество этажей;
* `height` - высота одного этажа в местрах;
* `velocity` - скорость лифта при движении в м/с;
* `timeout-doors` - время открытия/закрытия дверей лифта.

Команды для работы с лифтом:
* `call <n>` - вызов лифта на этаж из подъезда;
* `goto <n>` - нажать на кнопку этажа внутри лифта.

## Сборка и тестирование
Протестировано на Linux Ubuntu 17.10, собрано на gcc version 7.2.0.

## Лицензия
Данный проект создан под лицензией MIT - подробности можно посмотреть здесь [LICENSE](https://github.com/drobyshev/lift-simulator/blob/master/LICENSE).
