#include <iostream>
#include <Windows.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/Dbo/Session.h>
#include "Tables.h"


void createData(Wt::Dbo::Session& session)
{
	Wt::Dbo::Transaction transaction{ session };
	{
		std::unique_ptr<Publisher> publisher{ new Publisher() };
		publisher->name = "BHV";
		auto publisherPtr = session.add(std::move(publisher));

		std::unique_ptr<Book> book{ new Book() };
		book->title = "Perfect code";
		Wt::Dbo::ptr<Book> bookPtr = session.add(std::move(book));
		bookPtr.modify()->publisher_id = publisherPtr;

		std::unique_ptr<Book> book2{ new Book() };
		book2->title = "3ds Max";
		Wt::Dbo::ptr<Book> bookPtr2 = session.add(std::move(book2));
		bookPtr2.modify()->publisher_id = publisherPtr;

		std::unique_ptr<Shop> shop{ new Shop() };
		shop->name = "First shop";
		Wt::Dbo::ptr<Shop> shopPtr = session.add(std::move(shop));

		std::unique_ptr<Shop> shop2{ new Shop() };
		shop2->name = "Second shop";
		Wt::Dbo::ptr<Shop> shopPtr2 = session.add(std::move(shop2));

		std::unique_ptr<Stock> stock{ new Stock() };
		stock->count = 3;
		Wt::Dbo::ptr<Stock> stockPtr = session.add(std::move(stock));
		stockPtr.modify()->book = bookPtr;
		stockPtr.modify()->shop = shopPtr;

		std::unique_ptr<Stock> stock2{ new Stock() };
		stock2->count = 2;
		Wt::Dbo::ptr<Stock> stockPtr2 = session.add(std::move(stock2));
		stockPtr2.modify()->book = bookPtr2;
		stockPtr2.modify()->shop = shopPtr2;

		std::unique_ptr<Sale> sale{ new Sale };
		sale->count = 2;
		sale->date_sale = "07/07/2023";
		sale->price = 399;
		Wt::Dbo::ptr<Sale> salePtr = session.add(std::move(sale));
		salePtr.modify()->stock = stockPtr;

		transaction.commit();

	}
	Wt::Dbo::Transaction transaction2{ session };

	{
		std::unique_ptr<Publisher> publisher{ new Publisher() };
		publisher->name = "MIF";
		Wt::Dbo::ptr<Publisher> publisherPtr = session.add(std::move(publisher));

		std::unique_ptr<Book> book{ new Book() };
		book->title = "Stranger";
		Wt::Dbo::ptr<Book> bookPtr = session.add(std::move(book));
		bookPtr.modify()->publisher_id = publisherPtr;

		std::unique_ptr<Book> book2{ new Book() };
		book2->title = "Consultant";
		Wt::Dbo::ptr<Book> bookPtr2 = session.add(std::move(book2));
		bookPtr2.modify()->publisher_id = publisherPtr;

		std::unique_ptr<Book> book3{ new Book() };
		book3->title = "Necklace";
		Wt::Dbo::ptr<Book> bookPtr3 = session.add(std::move(book3));
		bookPtr3.modify()->publisher_id = publisherPtr;

		std::unique_ptr<Shop> shop3{ new Shop() };
		shop3->name = "Third shop";
		Wt::Dbo::ptr<Shop> shopPtr3 = session.add(std::move(shop3));

		std::unique_ptr<Shop> main_shop{ new Shop() };
		main_shop->name = "Main shop";
		Wt::Dbo::ptr<Shop> shopPtr = session.add(std::move(main_shop));

		std::unique_ptr<Stock> stock{ new Stock() };
		stock->count = 3;
		Wt::Dbo::ptr<Stock> stockPtr = session.add(std::move(stock));
		stockPtr.modify()->book = bookPtr;
		stockPtr.modify()->shop = shopPtr;

		std::unique_ptr<Stock> stock2{ new Stock() };
		stock2->count = 2;
		Wt::Dbo::ptr<Stock> stockPtr2 = session.add(std::move(stock2));
		stockPtr2.modify()->book = bookPtr2;
		stockPtr2.modify()->shop = shopPtr;

		std::unique_ptr<Stock> stock3{ new Stock() };
		stock3->count = 2;
		Wt::Dbo::ptr<Stock> stockPtr3 = session.add(std::move(stock3));
		stockPtr3.modify()->book = bookPtr3;
		stockPtr3.modify()->shop = shopPtr3;

		std::unique_ptr<Sale> sale{ new Sale };
		sale->count = 3;
		sale->date_sale = "07/07/2023";
		sale->price = 299;
		Wt::Dbo::ptr<Sale> salePtr = session.add(std::move(sale));
		salePtr.modify()->stock = stockPtr;

		std::unique_ptr<Sale> sale2{ new Sale };
		sale2->count = 1;
		sale2->date_sale = "11/11/2023";
		sale2->price = 499;
		Wt::Dbo::ptr<Sale> salePtr2 = session.add(std::move(sale2));
		salePtr2.modify()->stock = stockPtr2;


		transaction2.commit();
	}

}

void find(Wt::Dbo::Session& session, std::string publisher) {

	Wt::Dbo::Transaction transaction{ session };
	std::vector<Wt::Dbo::ptr<Shop>> res;
	std::string query = "SELECT DISTINCT s\
			FROM\
			shop s\
			JOIN stock st ON st.shop_id = s.id\
			JOIN book b ON st.book_id = b.id\
			JOIN publisher p ON b.publisher_id = p.id\
			WHERE p.name = ?";

	Wt::Dbo::collection<Wt::Dbo::ptr<Shop>> shops = session.query<Wt::Dbo::ptr<Shop>>(query).bind(publisher);
	if (shops.empty()) {

		std::cout << "Магазины не найдены" << std::endl;
	}
	else {
		for (const Wt::Dbo::ptr<Shop>& item : shops)
		{
			res.push_back(item);
			std::cout << item << std::endl;
		}
	}
}


int main() {

	setlocale(LC_ALL, "Russian");
	try {
		std::string connection(
			"host=localhost "
			"port=5432 "
			"dbname=bookstore "
			"user=postgres "
			"password=postgres "
			);
		auto postgres = std::make_unique<Wt::Dbo::backend::Postgres>(connection);
		Wt::Dbo::Session session;
		session.setConnection(std::move(postgres));
		session.mapClass<Publisher>("publisher");
		session.mapClass<Book>("book");
		session.mapClass<Shop>("shop");
		session.mapClass<Sale>("sale");
		session.mapClass<Stock>("stock");
		try {
			session.createTables();
		}
		catch (const Wt::Dbo::Exception& ex) {
			std::cout << "Exception: tables already exist" << std::endl;
		}
		createData(session);
		std::string publisher;
		std::cout << "Введите издателя: " << std::endl;
		std::cin >> publisher;
		find(session, publisher);
		
	}
	catch (const Wt::Dbo::Exception& ex) {
		std::cout << "Error when connecting to database: " << ex.what() << std::endl;
	}
	return 0;
}