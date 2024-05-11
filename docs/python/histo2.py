import matplotlib.pyplot as plt

y_name = "Накладные ресурсы (%)"
x_name = "Количество пар одновременно работающих ВМ"

max_y = 170
categories = ["2", "4 ", "6"]
values1 = [100, 100, 100]
values2 = [138, 142, 155]
index = range(len(categories))

bar_width = 0.35
fig, ax = plt.subplots()
bar1 = ax.bar(index, values1, 0.3, label='Аппаратная реализация', hatch='/', edgecolor='black', color='white')
bar1 = ax.bar([i + bar_width for i in index], values2, 0.3, label='Виртуализация', hatch='\\', edgecolor='black', color='white')

ax.set_ylabel(y_name)
ax.set_xlabel(x_name)
ax.set_xticks([i + bar_width/2 for i in index])
ax.set_xticklabels(categories)
ax.legend()

plt.gca().spines['top'].set_visible(True)
plt.yticks(range(0, max_y, 10))
plt.ylim(0, max_y)
plt.show()
