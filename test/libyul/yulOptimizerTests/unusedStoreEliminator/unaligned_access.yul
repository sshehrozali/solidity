{
    mstore(0, 0x1234)
    mstore(4, 0x456)
    revert(0, 0x24)
}
// ----
// step: unusedStoreEliminator
//
// {
//     {
//         mstore(0, 0x1234)
//         mstore(4, 0x456)
//         revert(0, 0x24)
//     }
// }
