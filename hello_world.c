#include <stdlib.h>
#include <stdio.h>

#include <avro.h>

// macros
// check for errors that return int
#ifndef check_i
#define check_i(call) \
  do { \
    if ((call) != 0) { \
      fprintf(stderr, "Error: %s\n", avro_strerror()); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)
#endif

// check for errors that return pointers
#ifndef check_p
#define check_p(call) \
  do { \
    if ((call) == NULL) { \
      fprintf(stderr, "Error: %s\n", avro_strerror()); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)
#endif

// print to stderr
#ifndef eprintf
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif


// function declarations
static void write_data(const char *out_filename, const char PERSON_SCHEMA[]);
static void write_person(avro_value_t *writer_value, avro_file_writer_t file,
  char *first, char *last, char *phone, char *bday, char *addy);
static void read_data(const char *in_filename);


int main(int argc, char **argv) {
  // declare some stuff
  const char PERSON_SCHEMA[] = 
    "{\"type\":\"record\",\
    \"name\":\"Person\",\
    \"fields\":[\
      {\"name\": \"First\", \"type\": \"string\"},\
      {\"name\": \"Last\", \"type\": \"string\"},\
      {\"name\": \"Phone\", \"type\": \"string\"},\
      {\"name\": \"Birthday\", \"type\": \"string\"},\
      {\"name\": \"Address\", \"type\": \"string\"}]}";
  const char *filename = "people.test.avro";

  // write to file
  write_data(filename, PERSON_SCHEMA);

  // read from file
  read_data(filename);

  return EXIT_SUCCESS;
}

static void write_person(avro_value_t *writer_value, avro_file_writer_t file,
  char *first, char *last, char *phone, char *bday, char *addy) {
  avro_value_t field; // stores path to a field in record

  // assign fields in record
  // first name
  check_i(avro_value_get_by_name(writer_value, "First", &field, NULL));
  check_i(avro_value_set_string(&field, first));
  // last name
  check_i(avro_value_get_by_name(writer_value, "Last", &field, NULL));
  check_i(avro_value_set_string(&field, last));
  // phone number
  check_i(avro_value_get_by_name(writer_value, "Phone", &field, NULL));
  check_i(avro_value_set_string(&field, phone));
  // birthday
  check_i(avro_value_get_by_name(writer_value, "Birthday", &field, NULL));
  check_i(avro_value_set_string(&field, bday));
  // address
  check_i(avro_value_get_by_name(writer_value, "Address", &field, NULL));
  check_i(avro_value_set_string(&field, addy));

  // write record to file
  check_i(avro_file_writer_append_value(file, writer_value));
  
  /* - testing how they store the fields in the record (writer_value)
     - values stay in the field unless changed
  // first name
  check_i(avro_value_get_by_name(writer_value, "First", &field, NULL));
  check_i(avro_value_set_string(&field, "Will"));
  // write record to file
  check_i(avro_file_writer_append_value(file, writer_value));
  */
}

static void write_data(const char *out_filename, const char PERSON_SCHEMA[]) {
  // parse schema into schema data structure
  avro_schema_t writer_schema;
  avro_schema_error_t error;
  check_i(avro_schema_from_json(PERSON_SCHEMA, 0, &writer_schema, &error));

  // create class from schema
  avro_value_iface_t *writer_iface;  // class
  check_p(writer_iface = avro_generic_class_from_schema(writer_schema));
  // create instance from that class
  avro_value_t writer_value; // instance
  check_i(avro_generic_value_new(writer_iface, &writer_value));
  
  // open new data file for writing
  //// also writes schema to the file here
  avro_file_writer_t file;
  check_i(avro_file_writer_create(out_filename, writer_schema, &file));

  // write records
  write_person(&writer_value, file, "Glen", "Smith", "3849007657", 
    "06-05-2003", "123 Peachtree Lane");
  write_person(&writer_value, file, "Josh", "Smith", "1234567809", 
    "?", "234 Peachtree Lane");
  write_person(&writer_value, file, "Sean", "Smith", "8374859172", 
    "12-05-1990", "345 Peachtree Lane");
  write_person(&writer_value, file, "Curtis", "Smith", "8394728199", 
    "1/4/99", "456 Peachtree Lane");
  write_person(&writer_value, file, "Jim", "Smith", "222-333-4444", 
    "12-05-1990", "N/A");
  write_person(&writer_value, file, "Blake", "Smith", "(372) 543-1111", 
    "12-05-1990", "678 Peachtree Lane");
  write_person(&writer_value, file, "Mac", "Smith", "(765)-108-7654", 
    "May 29 1989", "789 Peachtree Lane");
  write_person(&writer_value, file, "Niko", "Smith", "273 242 3432", 
    "1989-08-17", "890 Peachtree Lane");

  // clean up
  avro_file_writer_close(file);
  avro_value_decref(&writer_value);
  avro_value_iface_decref(writer_iface);
  avro_schema_decref(writer_schema);
}

static void read_data(const char *in_filename) {
  // open avro file and grab the schema stored in it
  avro_schema_t writer_schema;
  avro_file_reader_t file;
  check_i(avro_file_reader(in_filename, &file)); // opens file
  writer_schema = avro_file_reader_get_writer_schema(file); // get schema

  // create class from schema
  avro_value_iface_t *writer_iface; // class
  check_p(writer_iface = avro_generic_class_from_schema(writer_schema));
  // create instance from that class
  avro_value_t writer_value; // instance
  check_i(avro_generic_value_new(writer_iface, &writer_value));

  // print all people from file
  eprintf("%10s %10s %15s %15s %25s\n", "First Name", "Last Name", 
    "Phone Number", "Birthday", "Address");
  while (avro_file_reader_read_value(file, &writer_value) == 0) {
    // - assuming avro_file_reader_read_value advances "record pointer" to next
    // person
    // - stores a record into writer_value
    avro_value_t field; // a field in the record we are on
    const char *first;
    const char *last;
    const char *phone;
    const char *bday;
    const char *addy;
    size_t size;
  
    // first name
    check_i(avro_value_get_by_name(&writer_value, "First", &field, NULL)); 
    check_i(avro_value_get_string(&field, &first, &size)); 
    // last name
    check_i(avro_value_get_by_name(&writer_value, "Last", &field, NULL)); 
    check_i(avro_value_get_string(&field, &last, &size)); 
    // phone number
    check_i(avro_value_get_by_name(&writer_value, "Phone", &field, NULL)); 
    check_i(avro_value_get_string(&field, &phone, &size)); 
    // birthday
    check_i(avro_value_get_by_name(&writer_value, "Birthday", &field, NULL)); 
    check_i(avro_value_get_string(&field, &bday, &size)); 
    // address
    check_i(avro_value_get_by_name(&writer_value, "Address", &field, NULL)); 
    check_i(avro_value_get_string(&field, &addy, &size)); 

    eprintf("%10s %10s %15s %15s %25s\n", first, last, phone, bday, addy);
  }

  // clean up
  avro_value_decref(&writer_value);
  avro_value_iface_decref(writer_iface);
  avro_file_reader_close(file);
  avro_schema_decref(writer_schema);
}
