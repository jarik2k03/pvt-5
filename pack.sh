#!/bin/sh

tar -czf repo.tar.gz .
scp repo.tar.gz iv121s05@oak.cpct.sibsutis.ru:~/repo.tar.gz # свой логин
