// main.cpp

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

static int counter = 1;
static bool isStart = false;
static guint timerId = -1;
static GtkWidget *moneyLabel, *costEntry;

gchar * getMoneyString(int seconds, float cost) 
{
	float total = (seconds)*(cost/3600);
	g_print("Заработано  %f по ставке %f\n", total, cost);
	return g_strdup_printf("Заработано: %.2f рублей", total);
}

gboolean TimerFunc (gpointer data) 
{	
	int hours = counter / 3600;
	int minutes = (counter / 60) % 60;
	int seconds = counter % 60;
	//Устанавливаем время
	gchar *str = g_strdup_printf("%02d:%02d:%02d", hours, minutes, seconds);
	gtk_label_set_text(GTK_LABEL(GTK_WIDGET(label)), str);
	g_free(str);

	//получаем данные ставки
	const gchar *cost = gtk_entry_get_text(GTK_ENTRY(costEntry));
	float money = atof((char*)cost);
	//получаем и присваиваем отчет по заработанному
	str = getMoneyString(counter, money);
	gtk_label_set_text(GTK_LABEL(moneyLabel),str);
	g_free(str);

	counter++;
	//снова запускаем таймер
	return TRUE;

}

static void button_clicked(GtkWidget* widget, gpointer data)
{
	if (!isStart) {
		isStart = true;
		gtk_button_set_label(GTK_BUTTON(widget), "Приостановить");
		timerId = g_timeout_add_seconds(1,TimerFunc,GTK_LABEL(GTK_WIDGET(data)));
	} else {
		gtk_button_set_label(GTK_BUTTON(widget), "Продолжить");

		if (g_source_remove(timerId) == TRUE) {
			isStart = false;
			g_print("timer removed\n");
		} else {
			g_print("timer not removed\n");
		}
	}
}

int main (int argc, char* argv[]) 
{
	gtk_init(&argc, &argv); //инициализируем gtk

	GtkWidget *window,*button, *label, *hbox; //определяем необходимые переменные
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);//создаем окно
	//Устанавливаем заголовок окна
	gtk_window_set_title(GTK_WINDOW(window), "Freelance Timer");
	//Устанавливаем размер окна
	gtk_widget_set_size_request(window,200,200);
	//Вешаем на крестик сигнал того, что программа завершится
	g_signal_connect(window, "delete_event",G_CALLBACK(gtk_main_quit),NULL);


	/* ПРОГРАММА НАЧИНАЕТСЯ ЗДЕСЬ */
	button = gtk_button_new_with_label("Начать работу");
	label = gtk_label_new("00:00:00");
	gchar * str = getMoneyString(0,0);
	moneyLabel = gtk_label_new(str);
	g_free(str);

	costEntry = gtk_entry_new();

	// вешаем сигналы событий
	g_signal_connect(button,"clicked",G_CALLBACK(button_clicked),label);

	

	hbox = gtk_box_new(GtkOrientation::GTK_ORIENTATION_VERTICAL,0);
	gtk_box_pack_start(GTK_BOX(hbox),label,0,0,0);
	gtk_box_pack_start(GTK_BOX(hbox),moneyLabel,0,0,0);

	GtkWidget *moneyHbox = gtk_box_new(GtkOrientation::GTK_ORIENTATION_VERTICAL, 0);

	gtk_box_pack_start(GTK_BOX(moneyHbox),gtk_label_new("Введите ставку"),0,0,0);
	gtk_box_pack_start(GTK_BOX(moneyHbox),costEntry,0,0,0);

	gtk_box_pack_start(GTK_BOX(hbox),moneyHbox,0,0,0);
	gtk_box_pack_start(GTK_BOX(hbox),button,0,0,0);

	gtk_container_add(GTK_CONTAINER(window),hbox);
	
	gtk_widget_show_all(window); //показываем все виджеты
	gtk_main(); //запускаем главный цикл
	return 0;
}
