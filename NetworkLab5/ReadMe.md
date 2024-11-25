# Описание задачи
* Необходимо реализовать прокси-сервер, соответствующий стандарту SOCKS версии 5. Сервер должен:

* Принимать входящие подключения от клиентов на указанном порту.

* Реализовывать обязательную команду 1 (установление TCP/IP потокового соединения).

* Не поддерживать аутентификацию и IPv6-адреса.

* Использовать неблокирующиеся сокеты в рамках одного потока.

* Поддерживать передачу данных в обе стороны и аккуратно закрывать соединения.

* Не иметь холостых циклов и неограниченного расходования памяти.

* Обеспечивать производительность, сравнимую с прямым подключением.

* Поддерживать неблокирующийся резолвинг доменных имен.

* Требования к реализации
Неблокирующиеся сокеты: Использовать неблокирующиеся сокеты и работать с ними в рамках одного потока.

* Без холостых циклов: Не допускать ситуаций, когда тело цикла выполняется неоднократно без фактической передачи данных.

* Ограничение памяти: Не допускать неограниченного расходования памяти для обслуживания одного клиента.

* Производительность: Производительность работы через прокси не должна быть заметно хуже, чем без прокси.

* Резолвинг доменных имен: Использовать UDP-сокет для отправки DNS-запросов и обработки ответов.

Для тестирования можно настроить любой Web-браузер на использование вашего прокси, и посещать любые веб-сайты, богатые контентом. 
# Параметры запуска
При запуске сервера ничего делать не надо: 
``` bash
    ./NerworkLab5
```