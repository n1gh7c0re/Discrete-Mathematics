# Проверка свойства древочисленности (субцикличность)
На вход программе подаётся граф.

Проверить, является ли граф деревом, используя утверждение 7 из теоремы параграфа “Основные свойства свободных деревьев”. Если нет, то в выходной файл написать, что нарушено: ацикличность (в этом случае предоставить хотя бы один найденный цикл), субцикличность (в этом случае указать для какого ребра это неверно) или и то, и другое. В любом случае проверить, является ли граф древочисленным.

По итогу нужно запустить программу на различных графах и увидеть, что либо граф является деревом и древочисленный, либо то, что если ровно одно из условий из утверждения 7 не выполнено, то граф не будет древочисленным, либо то, что если оба условия не выполнены, то граф как может, так и не может быть древочисленным (всё перечисленное должно быть верно за исключениями из утверждения 6).

Здесь используются утверждения 5, 6 и 7 из теоремы параграфа “Основные свойства свободных деревьев”.

Дополнительные требования:
-	указать сложность алгоритма и доказать, что она именно такая;
-	объяснить почему был выбран тот или иной способ представления графов в программе.
