import pandas as pd

orders = pd.read_csv("orders.csv")
order_items = pd.read_csv("order_items.csv")
products = pd.read_csv("products.csv")
customers = pd.read_csv("customers.csv")

# join orders + order_items
df = pd.merge(order_items, orders, on="order_id")

# join products
df = pd.merge(df, products, on="product_id")

# join customers
df = pd.merge(df, customers, on="customer_id")

transactions = df[[
    "order_id",
    "customer_id",
    "product_id",
    "product_name",
    "category",
    "quantity",
    "order_date"
]]

transactions.rename(columns={"order_id":"transaction_id"}, inplace=True)

print(transactions.head())

transactions.to_csv("transactions_dataset.csv", index=False)