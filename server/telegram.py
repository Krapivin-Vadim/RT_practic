import telebot
from telebot import types
import os
import threading

TOKEN = '8161889524:AAFv77JSaUXnWGBWreJTC7UiEJpYZbYm_g8'
END = "@_end_@"
orders_file = 'orders.txt'


class Drink_offer_bot:
    def __init__(self, token):
        self.running = False
        self.TOKEN = token
        self.orders_file = 'orders.txt'
        self.orders = []
        if os.path.exists(self.orders_file):
            with open(orders_file, 'r', encoding='utf-8') as f:
                self.orders = [line.strip() for line in f.readlines()]

        self.bot = telebot.TeleBot(self.TOKEN)

        self.bot.message_handler(
            commands=['start'])(lambda message: self.send_welcome(message))

        self.bot.message_handler(
            commands=['orders'])(lambda message: self.show_orders(message))

        self.bot.message_handler(func=lambda message: True, content_types=['text'])(
            lambda message: self.handle_message(message))

    def save_order_to_file(self, order):
        with open(orders_file, 'a', encoding='utf-8') as f:
            f.write(order + '\n')

    def send_welcome(self, message):
        markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
        button = types.KeyboardButton("СДЕЛАТЬ ЗАКАЗ")
        markup.add(button)
        self.bot.send_message(
            message.chat.id, "Привет! Нажми кнопку ниже, чтобы сделать заказ.", reply_markup=markup)

    def show_orders(self, message):
        if self.orders:
            response = "Список заказов:\n" + \
                "\n".join(f"{idx+1}. {order}" for idx,
                          order in enumerate(self.orders))
        else:
            response = "Пока нет ни одного заказа."
        self.bot.reply_to(message, response)

    def handle_message(self, message):
        if message.text.startswith('/'):
            return
        if message.text.strip() == END:
            self.bot.reply_to(message, "Бот останавливается")
            self.bot.stop_polling()
            self.running = False
            return
        order = message.text.strip()
        self.orders.append(order)
        # save_order_to_file(order)
        self.bot.reply_to(message, "Заказ принят!")

    def run_bot(self):
        # print(self.bot)
        self.running = True
        self.bot.polling()

    def pop_order(self):
        if not len(self.orders) != 0:
            return None
        return self.orders.pop(0)


if __name__ == "__main__":
    bot = Drink_offer_bot(TOKEN)
    t = threading.Thread(target=bot.run_bot)
    t.start()
    t.join()
    print("stoped")
