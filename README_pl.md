# Projekt Kostki RGB z wymiarami 5 na 5 na 5
Krzysztof Reczek EiT Rok 3

# Opis projektu :
Celem projektu jest stworzenie gry “snake” i przedstawienie jej na kostce LED 5x5x5
składającej się z diod RGB WS2811. W ramach projektu zostanie przygotowana biblioteka,
która pozwoli na obsługę każdej diody ze 125 i ustawienie wybranego koloru. Do
komunikacji z kostką będziemy korzystać z pinów GPIO. Projekt będzie można rozbudować
o dodatkowe wizualne animacje i gry poprzez dodanie możliwości przełączania się
pomiędzy trybami.

# Analiza problemu:
Diody RGB WS2811 posiadają 4 piny kolejno od najdłuższego do najkrótszego:
- zasilanie układu wynoszące 5V
- masę układu
- DIN - wejście sterujące, którego używamy do sterowania stanem diody i jej kolorem
- DOUT - wyprowadzenie, które podłączamy na wejście kolejnej diody, nie musi zostać
użyte
Kolor i barwa diody są przesyłane w 3 liczbach RGB, z których każda składa z 8 bitów.
Oznacza to, że aby w pełni zakodować kolor musi zostać przesłane 24 bity. Ważne jest to,
że pierwsza sekwencja dotyczy diody umieszczonej najbliżej mikrokontrolera, a każda
kolejna następujących po sobie diod w łańcuchu. Powoduje to, że aby zmienić stan
pojedynczej diody musimy podać dane dla wszystkich diod w linii.
Klawiatura matrycowa posiada 16 przycisków. Sześć przycisków użytych zostanie do sterowania “wężem” w 3
wymiarach.

# Plan realizacji:
Pierwszym krokiem będzie zlutowanie wszystkich diod RGB w kostkę LED 5x5x5.
Następnie zrealizowane będzie podłączenie płytki FRDM-KL05Z z kostką LED i klawiaturą
matrycową 4x4. Po podłączeniu wszystkich komponentów sprawdzane będzie działanie
programu.
W ramach projektu będzie stworzona biblioteka do kostki LED 5x5x5 oraz zostanie
wykorzystana biblioteka z laboratoriów, która służy do obsługi klawiatury matrycowej 4x4.
