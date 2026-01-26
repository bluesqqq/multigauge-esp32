struct Image {
    int width = 0;
    int height = 0;

    void* native = nullptr;

    enum class Type { Unknown, LGFX } type = Type::Unknown;
};