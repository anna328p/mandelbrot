#include <gtkmm.h>
#include <iostream>

Gtk::Window *window0 = nullptr;

static void on_button_clicked() {
	std::cout << "Click!" << std::endl;
}

int main(int argc, char **argv) {
	auto app = Gtk::Application::create(argc, argv, "xyz.dkudriavtsev.mandelbrot");

	auto refBuilder = Gtk::Builder::create();

	try {
		refBuilder->add_from_file("gui.glade");
	} catch (const Glib::FileError& ex) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	} catch (const Glib::MarkupError& ex) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	} catch (const Gtk::BuilderError& ex) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
	}

	refBuilder->get_widget("window0", window0);
	if (window0) {
		//Get the GtkBuilder-instantiated Button, and connect a signal handler:
		Gtk::Button* btnExecute = nullptr;
		refBuilder->get_widget("btnExecute", btnExecute);
		if (btnExecute) {
			btnExecute->signal_clicked().connect(sigc::ptr_fun(on_button_clicked));
		}

		app->run(*window0);
	}

	delete window0;
	return 0;
}

