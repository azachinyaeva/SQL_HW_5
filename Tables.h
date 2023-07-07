#pragma once
#include <Wt/Dbo/Dbo.h>

class Book;
class Stock;
class Sale;

class Publisher {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Book>> books;
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "publisher");
	}

};

class Book {
public:
	std::string title = "";
	Wt::Dbo::ptr<Publisher> publisher_id;
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, publisher_id, "publisher");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "book");

	}
};

class Shop {
public:
	std::string name = "";
	Wt::Dbo::collection<Wt::Dbo::ptr<Stock>> stocks;
	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "shop");

	}

};

class Stock {
public:
	int count = 0;
	Wt::Dbo::ptr<Book> book;
	Wt::Dbo::ptr<Shop> shop;
	Wt::Dbo::collection<Wt::Dbo::ptr<Sale>> sales;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, book, "book");
		Wt::Dbo::belongsTo(a, shop, "shop");
		Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "stock");

	}
};

class Sale {
public:
	int price = 0;
	std::string date_sale = "";
	int count = 0;
	Wt::Dbo::ptr<Stock> stock;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, price, "price");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::belongsTo(a, stock, "stock");
	}
};
