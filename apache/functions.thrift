namespace cpp functions

//Serviço de adição de longs

struct AddRequest {
  1: i64 num1;
  2: i64 num2;
  3: i64 num3;
  4: i64 num4;
  5: i64 num5;
  6: i64 num6;
  7: i64 num7;
  8: i64 num8;
}

struct User {
  1: i32 id;
  2: string name;
  3: string age;
  4: string email;
}

service ApacheService {
   i64 add (1:AddRequest request)
   void printrandom()
   i64 fibonacci(1:i64 input)
   string onlydigits(1:string input)
   User getuser(1:i32 id)

}