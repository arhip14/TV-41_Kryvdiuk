# 🧠 **Розв'язувач Головоломки з Доміно — Завдання №16**

> ✨ **Ідеальне рішення для розв'язання інтелектуальної головоломки з доміно** на мові **ANSI C++**.  
> 🧩 **Грай** — **Розв'язуй** — **Автоматизуй**.

---

## 📌 **Опис задачі**

Цей проект містить алгоритм для розв'язання головоломки з доміно, де кожне доміно на полі з'являється **рівно один раз**. Ваше завдання — **відновити стерті числа** та **знайти правильне розташування кожного доміно**.

### **Основні моменти:**
- Відоме ігрове поле (грид), частина чисел на якому стерта.
- Потрібно розставити доміно так, щоб:
  - Кожна пара чисел з'являлась лише один раз.
  - Усі доміно були правильно розміщені на полі.
  - Не було дублювання чисел.

🔍 **Приклад** у завданні допоможе краще зрозуміти правила гри та хід розв'язку.

---

## 🚀 **Технології**

- 💻 **Мова програмування**: **ANSI C++**
- 📦 **Структура**: консольна програма з алгоритмом розпізнавання та заповнення
- 🧪 **Вхідні дані**: ігрове поле (масив чисел з пропусками)
- 📤 **Вихід**: оновлене поле з усіма знайденими доміно

---

## 📂 **Структура репозиторію**

📁 `/domino-solver/`  
├── 📄 **main.cpp**         # Головний файл з реалізацією  
├── 📄 **utils.h/.cpp**     # Допоміжні функції (читання, виведення, перевірки)  
├── 📄 **README.md**        # Опис проекту  
├── 📄 **Завдання №16.pdf** # Оригінал завдання

---

## 🧠 **Логіка рішення**
   ├──🔁 Перебір усіх можливих варіантів розташування.
   ├──✅ Перевірка кожного кроку на допустимість.
   ├──🔙 Backtracking для відкату й пошуку правильного рішення.
   ├──🧩 Збір усіх можливих рішень (або єдиного, якщо таке є).

##📚 **Корисні ресурси**
   •🔗 [Опис завдання (PDF)](Завдання №16.pdf)
   
##✍️ **Автор**
Кривдюк Архип Валерійович — студент ІАТЕ КПІ
