
namespace utils {
	struct CastStruct {
		
	}
	Cas;
}

/*

Класс Cas
	Компонент Compare ("123" == указатель на аналогичную строку)
		Компонент Weak ("123" == 123)
		Компонент Strong (123 == 123)
	Компонент to<T>
	Компонент ErrorFunction вида
		int func();
	
	
int a = Cas.to<int>("hello"); //Error
int b = Cas.to<int>("315.2"); //Error
int c = Cas.to<int>(Cas.to<double>("315.2"));

*/